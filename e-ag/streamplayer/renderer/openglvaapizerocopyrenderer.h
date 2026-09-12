#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMutex>
#include <QGuiApplication>
#include <QHash>
#include <unistd.h>
#include "../common/hardwareframe.h"
#include "../common/vaapihardwareframe.h"
#include "../common/MediaFrame.h"

extern "C" {
#include <va/va.h>
#include <va/va_drmcommon.h>
}

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "AbstractVideoRenderer.h"


class OpenGLVaapiZeroCopyRenderer :
    public AbstractVideoRenderer,
    protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit OpenGLVaapiZeroCopyRenderer(QWidget *parent = nullptr);

    ///void setFrame(const MediaFrame &frame) override;

public slots:
    void onFrame(const MediaFrame &frame);

    bool importSurface(VaapiHardwareFrame *frame);

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    MediaFrame m_frame;
    QMutex m_mutex;

    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;

    PFNEGLCREATEIMAGEKHRPROC m_createImage = nullptr;
    PFNEGLDESTROYIMAGEKHRPROC m_destroyImage = nullptr;

    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC m_imageTargetTexture = nullptr;

    // Y texture
    GLuint m_yTexture = 0;

    // UV texture
    GLuint m_uvTexture = 0;

    // EGL images
    EGLImageKHR m_yImage = EGL_NO_IMAGE_KHR;
    EGLImageKHR m_uvImage = EGL_NO_IMAGE_KHR;

    QOpenGLShaderProgram *m_shader = nullptr;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    int m_videoWidth = 0;
    int m_videoHeight = 0;
    struct CachedSurface
    {
        EGLImageKHR yImage = EGL_NO_IMAGE_KHR;
        EGLImageKHR uvImage = EGL_NO_IMAGE_KHR;
        bool valid = false;
    };

    QHash<VASurfaceID, CachedSurface> m_surfaceCache;

    VASurfaceID m_currentSurface = VA_INVALID_SURFACE;
};
