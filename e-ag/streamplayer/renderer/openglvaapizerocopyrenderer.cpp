#include "openglvaapizerocopyrenderer.h"

#include <QDebug>
#include <QMutexLocker>

extern "C"
{
#include <libavutil/frame.h>
}

OpenGLVaapiZeroCopyRenderer::
OpenGLVaapiZeroCopyRenderer(QWidget *parent)
    : AbstractVideoRenderer(parent)
{
qDebug()<<"OpenGLVaapiZeroCopyRenderer";
}

void OpenGLVaapiZeroCopyRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    qDebug() << "VAAPI ZeroCopy Renderer initialized";

    m_eglDisplay = eglGetCurrentDisplay();

    m_createImage =
        reinterpret_cast<PFNEGLCREATEIMAGEKHRPROC>(
            eglGetProcAddress("eglCreateImageKHR"));

    m_destroyImage =
        reinterpret_cast<PFNEGLDESTROYIMAGEKHRPROC>(
            eglGetProcAddress("eglDestroyImageKHR"));

    m_imageTargetTexture =
        reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(
            eglGetProcAddress("glEGLImageTargetTexture2DOES"));

    qDebug() << "EGL Display =" << m_eglDisplay;
    qDebug() << "eglCreateImageKHR =" << (void*)m_createImage;
    qDebug() << "eglDestroyImageKHR =" << (void*)m_destroyImage;
    qDebug() << "glEGLImageTargetTexture2DOES ="
             << (void*)m_imageTargetTexture;

    qDebug() << "vendor"
             << glGetString(GL_VENDOR);

    qDebug() << "renderer"
             << glGetString(GL_RENDERER);

    qDebug() << "version"
             << glGetString(GL_VERSION);

    QOpenGLContext *ctx = context();

    qDebug() << "ctx" << ctx;
    qDebug() << "ctx->isOpenGLES()" << ctx->isOpenGLES();
    qDebug() << "QGuiApplication::platformName"
             << QGuiApplication::platformName();


    // --------------------------------------------------
    // Y texture
    // --------------------------------------------------

    glGenTextures(1, &m_yTexture);

    glBindTexture(GL_TEXTURE_2D, m_yTexture);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);


    // --------------------------------------------------
    // UV texture
    // --------------------------------------------------

    glGenTextures(1, &m_uvTexture);

    glBindTexture(GL_TEXTURE_2D, m_uvTexture);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE);


    // --------------------------------------------------
    // Shader
    // --------------------------------------------------

    m_shader = new QOpenGLShaderProgram(this);

    const char *vertexShader = R"(
        #version 330 core

        out vec2 vTexCoord;

        const vec2 positions[4] = vec2[](
            vec2(-1.0, -1.0),
            vec2( 1.0, -1.0),
            vec2(-1.0,  1.0),
            vec2( 1.0,  1.0)
        );

        const vec2 texcoords[4] = vec2[](
            vec2(0.0, 1.0),
            vec2(1.0, 1.0),
            vec2(0.0, 0.0),
            vec2(1.0, 0.0)
        );

        void main()
        {
            gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
            vTexCoord = texcoords[gl_VertexID];
        }
    )";


    const char *fragmentShader = R"(
        #version 330 core

        in vec2 vTexCoord;

        uniform sampler2D texY;
        uniform sampler2D texUV;

        out vec4 fragColor;

        void main()
        {
            float y = texture(texY, vTexCoord).r;
            vec2 uv = texture(texUV, vTexCoord).rg;

            y = 1.1643 * (y - 0.0625);

            float u = uv.x - 0.5;
            float v = uv.y - 0.5;

            float r = y + 1.5958 * v;
            float g = y - 0.39173 * u - 0.81290 * v;
            float b = y + 2.017 * u;

            fragColor = vec4(r, g, b, 1.0);
        }
    )";


    if (!m_shader->addShaderFromSourceCode(
            QOpenGLShader::Vertex,
            vertexShader))
    {
        qDebug() << "Vertex shader error:"
                 << m_shader->log();
    }

    if (!m_shader->addShaderFromSourceCode(
            QOpenGLShader::Fragment,
            fragmentShader))
    {
        qDebug() << "Fragment shader error:"
                 << m_shader->log();
    }

    if (!m_shader->link())
    {
        qDebug() << "Shader link error:"
                 << m_shader->log();
    }
}


void OpenGLVaapiZeroCopyRenderer::onFrame(const MediaFrame &frame)
{
 /*   qDebug()
        << "onFrame:"
        << "hardwareFrame ="
        << !frame.hardwareFrame
        << "width ="
        << frame.width
        << "height ="
        << frame.height
        << "pts ="
        << frame.pts;*/

    {
        QMutexLocker locker(&m_mutex);
        m_frame = frame;

       /* qDebug()
            << "stored:"
            << "hardwareFrame ="
            << !m_frame.hardwareFrame;*/
    }

    update();
}
void OpenGLVaapiZeroCopyRenderer::paintGL()
{


       glClear(GL_COLOR_BUFFER_BIT);

       QMutexLocker locker(&m_mutex);

      /* qDebug()
           << "PAINT:"
           << "hardwareFrame ="
           << static_cast<bool>(m_frame.hardwareFrame)
           << "width ="
           << m_frame.width
           << "height ="
           << m_frame.height
           << "pts ="
           << m_frame.pts;*/

       if (!m_frame.hardwareFrame)
       {
           qDebug() << "PAINT: NO HARDWARE FRAME";
           return;
       }

       auto hw =
           static_cast<VaapiHardwareFrame*>(
               m_frame.hardwareFrame.data());

      /// qDebug() << "PAINT hw =" << hw;

       if (!hw)
       {
           qDebug() << "PAINT: hw NULL";
           return;
       }

       ///qDebug() << "PAINT hw->frame =" << hw->frame;

       if (!hw->frame)
       {
           qDebug() << "PAINT: hw->frame NULL";
           return;
       }

     ///  qDebug() << "PAINT surface =" << hw->surface;


   // if (!importSurface(hw))
    //    return;
    bool ok = importSurface(hw);

    if (!ok)
    {
        qDebug() << "IMPORT SURFACE FAILED:"
                 << hw->surface;

        return;
    }

    // --------------------------------------------------
    // Shader
    // --------------------------------------------------

    m_shader->bind();


    glActiveTexture(GL_TEXTURE0);

    glBindTexture(
        GL_TEXTURE_2D,
        m_yTexture);

    m_shader->setUniformValue(
        "texY",
        0);


    glActiveTexture(GL_TEXTURE1);

    glBindTexture(
        GL_TEXTURE_2D,
        m_uvTexture);

    m_shader->setUniformValue(
        "texUV",
        1);


    // --------------------------------------------------
    // Full screen quad
    // --------------------------------------------------

    glDrawArrays(
        GL_TRIANGLE_STRIP,
        0,
        4);


    glBindTexture(
        GL_TEXTURE_2D,
        0);

    m_shader->release();
}

bool OpenGLVaapiZeroCopyRenderer::importSurface(
    VaapiHardwareFrame *hw)
{
    if (!hw)
        return false;

    if (hw->surface == VA_INVALID_SURFACE)
        return false;

    const VASurfaceID surface = hw->surface;

    // --------------------------------------------------
    // Daha önce import edilmiş mi?
    // --------------------------------------------------

    auto it = m_surfaceCache.find(surface);

    if (it != m_surfaceCache.end() &&
        it->valid)
    {
        static int cacheHit = 0;

      /*  if (++cacheHit % 30 == 0)
            qDebug() << "EGL CACHE HIT surface =" << surface;*/

        // Aynı VA surface.
        // EGLImage'ları tekrar oluşturma.
        if (m_currentSurface != surface)
        {
            glBindTexture(
                GL_TEXTURE_2D,
                m_yTexture);

            m_imageTargetTexture(
                GL_TEXTURE_2D,
                it->yImage);

            glBindTexture(
                GL_TEXTURE_2D,
                m_uvTexture);

            m_imageTargetTexture(
                GL_TEXTURE_2D,
                it->uvImage);

            glBindTexture(
                GL_TEXTURE_2D,
                0);

            m_currentSurface = surface;
        }

        return true;
    }


    // --------------------------------------------------
    // Yeni VA surface
    // --------------------------------------------------

    VADRMPRIMESurfaceDescriptor desc;
    memset(&desc, 0, sizeof(desc));

    VAStatus st = vaExportSurfaceHandle(
        hw->display,
        surface,
        VA_SURFACE_ATTRIB_MEM_TYPE_DRM_PRIME_2,
        VA_EXPORT_SURFACE_READ_ONLY |
        VA_EXPORT_SURFACE_SEPARATE_LAYERS,
        &desc);

    if (st != VA_STATUS_SUCCESS)
    {
        qDebug()
            << "vaExportSurfaceHandle failed:"
            << vaErrorStr(st);

        return false;
    }

    if (desc.num_layers < 2 ||
        desc.num_objects < 1)
    {
        qDebug()
            << "Invalid DRM PRIME descriptor:"
            << desc.num_layers
            << desc.num_objects;

        return false;
    }


    const int width  = hw->frame->width;
    const int height = hw->frame->height;

    const auto &yLayer  = desc.layers[0];
    const auto &uvLayer = desc.layers[1];

    const uint32_t yObject =
        yLayer.object_index[0];

    const uint32_t uvObject =
        uvLayer.object_index[0];

    const int fd =
        desc.objects[yObject].fd;


    // --------------------------------------------------
    // Y EGLImage
    // --------------------------------------------------

    EGLint yAttrs[] =
    {
        EGL_WIDTH,
        width,

        EGL_HEIGHT,
        height,

        EGL_LINUX_DRM_FOURCC_EXT,
        static_cast<EGLint>(
            yLayer.drm_format),

        EGL_DMA_BUF_PLANE0_FD_EXT,
        fd,

        EGL_DMA_BUF_PLANE0_OFFSET_EXT,
        static_cast<EGLint>(
            yLayer.offset[0]),

        EGL_DMA_BUF_PLANE0_PITCH_EXT,
        static_cast<EGLint>(
            yLayer.pitch[0]),

        EGL_NONE
    };

    EGLImageKHR yImage =
        m_createImage(
            m_eglDisplay,
            EGL_NO_CONTEXT,
            EGL_LINUX_DMA_BUF_EXT,
            nullptr,
            yAttrs);

    if (yImage == EGL_NO_IMAGE_KHR)
    {
        qDebug()
            << "Y EGLImage failed:"
            << Qt::hex
            << eglGetError();

        ::close(fd);
        return false;
    }


    // --------------------------------------------------
    // UV EGLImage
    // --------------------------------------------------

    const int uvFd =
        desc.objects[uvObject].fd;

    EGLint uvAttrs[] =
    {
        EGL_WIDTH,
        width / 2,

        EGL_HEIGHT,
        height / 2,

        EGL_LINUX_DRM_FOURCC_EXT,
        static_cast<EGLint>(
            uvLayer.drm_format),

        EGL_DMA_BUF_PLANE0_FD_EXT,
        uvFd,

        EGL_DMA_BUF_PLANE0_OFFSET_EXT,
        static_cast<EGLint>(
            uvLayer.offset[0]),

        EGL_DMA_BUF_PLANE0_PITCH_EXT,
        static_cast<EGLint>(
            uvLayer.pitch[0]),

        EGL_NONE
    };

    EGLImageKHR uvImage =
        m_createImage(
            m_eglDisplay,
            EGL_NO_CONTEXT,
            EGL_LINUX_DMA_BUF_EXT,
            nullptr,
            uvAttrs);

    if (uvImage == EGL_NO_IMAGE_KHR)
    {
        qDebug()
            << "UV EGLImage failed:"
            << Qt::hex
            << eglGetError();

        m_destroyImage(
            m_eglDisplay,
            yImage);

        ::close(fd);

        if (uvFd != fd)
            ::close(uvFd);

        return false;
    }


    // --------------------------------------------------
    // Cache
    // --------------------------------------------------

    CachedSurface cached;

    cached.yImage = yImage;
    cached.uvImage = uvImage;
    //cached.yFd = fd;
   //cached.uvFd = uvFd;
    cached.valid = true;

    m_surfaceCache.insert(
        surface,
        cached);


    // --------------------------------------------------
    // Texture'lara bağla
    // --------------------------------------------------

    glBindTexture(
        GL_TEXTURE_2D,
        m_yTexture);

    m_imageTargetTexture(
        GL_TEXTURE_2D,
        yImage);


    glBindTexture(
        GL_TEXTURE_2D,
        m_uvTexture);

    m_imageTargetTexture(
        GL_TEXTURE_2D,
        uvImage);


    glBindTexture(
        GL_TEXTURE_2D,
        0);


    m_currentSurface = surface;

    return true;
}

