#include "openglvideorenderer.h"

#include <QMutexLocker>

OpenGLVideoRenderer::OpenGLVideoRenderer(QWidget *parent)
    : AbstractVideoRenderer(parent)
{
   // setMinimumSize(640,480);
    qDebug()<<"OpenGLVideoRenderer";
}
OpenGLVideoRenderer::~OpenGLVideoRenderer()
{
    makeCurrent();

    releaseTextures();

    doneCurrent();
}

void OpenGLVideoRenderer::clear()
{
    {
        QMutexLocker locker(&m_mutex);

        m_frame = MediaFrame();
    }

    update();
}
/*
void OpenGLVideoRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    if(!initShaders())
    {
        qCritical() << "Shader initialization failed.";
        return;
    }

    if(!initGeometry())
    {
        qCritical() << "Geometry initialization failed.";
        return;
    }

    qDebug() << "OpenGL initialized";
    qDebug() << "Vendor  :" << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    qDebug() << "Renderer:" << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    qDebug() << "Version :" << reinterpret_cast<const char*>(glGetString(GL_VERSION));
}
*/
void OpenGLVideoRenderer::initializeGL()
{
    //qDebug() << "========== initializeGL =========="<< this;

    initializeOpenGLFunctions();

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    if(!initShaders())
    {
        qCritical() << "Shader initialization failed.";
        return;
    }

    if(!initGeometry())
    {
        qCritical() << "Geometry initialization failed.";
        return;
    }

    qDebug() << "OpenGL initialized";
}
void OpenGLVideoRenderer::resizeGL(int w,int h)
{
    glViewport(0,0,w,h);
}

void OpenGLVideoRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QMutexLocker locker(&m_mutex);

    //------------------------------------------------
    // Yeni yol (VAAPI Hardware Frame)
    //------------------------------------------------

    if (m_frame.hardwareFrame)
    {
        qDebug() << "Hardware frame";

        // Buraya birazdan geleceğiz.
        return;
    }

    //------------------------------------------------
    // Eski yol
    //------------------------------------------------

    if (!m_frame.buffer)
        return;

    if (!m_frame.buffer->isValid())
        return;

    m_program.bind();

    uploadTextures();

    if (m_frame.format == PixelFormat::YUV420P)
    {
        glUniform1i(m_locPixelFormat,0);

        glUniform1i(m_locTexY,0);
        glUniform1i(m_locTexU,1);
        glUniform1i(m_locTexV,2);
    }
    else if (m_frame.format == PixelFormat::NV12)
    {
        glUniform1i(m_locPixelFormat,1);

        glUniform1i(m_locTexY,0);
        glUniform1i(m_locTexUV,1);
    }

    m_vao.bind();

    glDrawArrays(GL_TRIANGLES,0,6);

    m_vao.release();

    m_program.release();
}
bool OpenGLVideoRenderer::initShaders()
{
    m_program.removeAllShaders();

    static const char *vertexShader = R"(

#version 330 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texcoord;

out vec2 TexCoord;

void main()
{
    TexCoord = texcoord;
    gl_Position = vec4(position,0.0,1.0);
}

)";

    static const char *fragmentShader = R"(

#version 330 core

in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D texY;
uniform sampler2D texU;
uniform sampler2D texV;
uniform sampler2D texUV;

uniform int pixelFormat;
void main()
{
    float y;
    float u;
    float v;

    y = texture(texY, TexCoord).r;

    if(pixelFormat == 0)
    {
        u = texture(texU, TexCoord).r - 0.5;
        v = texture(texV, TexCoord).r - 0.5;
    }
    else
    {
        vec2 uv = texture(texUV, TexCoord).rg;
        u = uv.r - 0.5;
        v = uv.g - 0.5;
    }

    vec3 rgb;

    rgb.r = y + 1.402 * v;
    rgb.g = y - 0.344136 * u - 0.714136 * v;
    rgb.b = y + 1.772 * u;

    FragColor = vec4(rgb,1.0);
}
)";

    if(!m_program.addShaderFromSourceCode(
            QOpenGLShader::Vertex,
            vertexShader))
    {
        qCritical() << m_program.log();
        return false;
    }

    if(!m_program.addShaderFromSourceCode(
            QOpenGLShader::Fragment,
            fragmentShader))
    {
        qCritical() << m_program.log();
        return false;
    }

    if(!m_program.link())
    {
        qCritical() << m_program.log();
        return false;
    }

    m_locTexY = m_program.uniformLocation("texY");
    m_locTexU = m_program.uniformLocation("texU");
    m_locTexV = m_program.uniformLocation("texV");
    m_locTexUV = m_program.uniformLocation("texUV");

    m_locPixelFormat =
            m_program.uniformLocation("pixelFormat");

    qDebug() << "Shader linked.";

    return true;
}

bool OpenGLVideoRenderer::initGeometry()
{
    static const GLfloat vertices[] =
    {
        // x      y        u     v

        -1.f,-1.f,   0.f,1.f,
         1.f,-1.f,   1.f,1.f,
        -1.f, 1.f,   0.f,0.f,

         1.f,-1.f,   1.f,1.f,
         1.f, 1.f,   1.f,0.f,
        -1.f, 1.f,   0.f,0.f
    };

    if(!m_vao.create())
        return false;

    if(!m_vbo.create())
        return false;

    m_vao.bind();

    m_vbo.bind();

    m_vbo.allocate(vertices,sizeof(vertices));

    m_program.bind();

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
                0,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat)*4,
                nullptr);

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
                1,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat)*4,
                reinterpret_cast<void*>(sizeof(GLfloat)*2));

    m_program.release();

    m_vbo.release();

    m_vao.release();

    qDebug() << "Geometry initialized";

    return true;
}

bool OpenGLVideoRenderer::initTextures(int width, int height)
{
    releaseTextures();

    m_textureWidth  = width;
    m_textureHeight = height;

    glGenTextures(1, &m_texY);
    glGenTextures(1, &m_texUV);
    glGenTextures(1, &m_texU);
    glGenTextures(1, &m_texV);
    auto createTexture =
            [this](GLuint tex,
               int w,
               int h)
    {
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_R8,
                     w,
                     h,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     nullptr);
    };

    createTexture(m_texY,width,height);

    glBindTexture(GL_TEXTURE_2D, m_texUV);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RG8,
                 width / 2,
                 height / 2,
                 0,
                 GL_RG,
                 GL_UNSIGNED_BYTE,
                 nullptr);


    createTexture(m_texU,width/2,height/2);

    createTexture(m_texV,width/2,height/2);

    m_textureInitialized = true;

    qDebug()<<"Textures created"
            <<width
            <<"x"
            <<height;

    return true;
}

void OpenGLVideoRenderer::releaseTextures()
{
    if(m_texY)
        glDeleteTextures(1,&m_texY);

    if(m_texUV)
        glDeleteTextures(1,&m_texUV);

    if(m_texU)
        glDeleteTextures(1,&m_texU);

    if(m_texV)
        glDeleteTextures(1,&m_texV);



    m_texY = 0;
    m_texUV = 0;
    m_texU = 0;
    m_texV = 0;

    m_textureInitialized = false;
}

void OpenGLVideoRenderer::uploadTextures()
{
    if (!m_frame.buffer)
        return;

    MediaBuffer *buf = m_frame.buffer.data();

    if (!buf->isValid())
        return;

    if (!m_textureInitialized ||
        m_textureWidth  != m_frame.width ||
        m_textureHeight != m_frame.height)
    {
        initTextures(m_frame.width,
                     m_frame.height);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    //------------------------------------------------------
    // Y plane (hem YUV420P hem NV12 ortak)
    //------------------------------------------------------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texY);

    glPixelStorei(GL_UNPACK_ROW_LENGTH,
                  buf->stride[0]);

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    m_frame.width,
                    m_frame.height,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    buf->plane[0]);

    //------------------------------------------------------
    // YUV420P
    //------------------------------------------------------

    if (m_frame.format == PixelFormat::YUV420P)
    {
        // U

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texU);

        glPixelStorei(GL_UNPACK_ROW_LENGTH,
                      buf->stride[1]);

        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        m_frame.width / 2,
                        m_frame.height / 2,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        buf->plane[1]);

        // V

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_texV);

        glPixelStorei(GL_UNPACK_ROW_LENGTH,
                      buf->stride[2]);

        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        m_frame.width / 2,
                        m_frame.height / 2,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        buf->plane[2]);
    }

    //------------------------------------------------------
    // NV12
    //------------------------------------------------------

    else if (m_frame.format == PixelFormat::NV12)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texUV);

        glPixelStorei(GL_UNPACK_ROW_LENGTH,
                      buf->stride[1] / 2);

        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        m_frame.width / 2,
                        m_frame.height / 2,
                        GL_RG,
                        GL_UNSIGNED_BYTE,
                        buf->plane[1]);
    }

    glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
}

void OpenGLVideoRenderer::onFrame(const MediaFrame &frame)
{
    {
        QMutexLocker locker(&m_mutex);

        m_frame = frame;
    }

    update();
}
