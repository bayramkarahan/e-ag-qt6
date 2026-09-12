#ifndef OPENGLVIDEORENDERER_H
#define OPENGLVIDEORENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QMutex>

#include "../common/MediaFrame.h"
#include "AbstractVideoRenderer.h"
class OpenGLVideoRenderer :
        public AbstractVideoRenderer,
        protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLVideoRenderer(QWidget *parent = nullptr);

    ~OpenGLVideoRenderer() override;
public slots:

    void onFrame(const MediaFrame &frame);

    void clear();

protected:

    void initializeGL() override;

    void resizeGL(int w,
                  int h) override;

    void paintGL() override;

private:

    bool initShaders();

    bool initGeometry();

    bool initTextures(int width,
                      int height);

    void uploadTextures();

    void releaseTextures();

private:

    QMutex m_mutex;

    MediaFrame m_frame;

    //----------------------------------------------------
    // OpenGL
    //----------------------------------------------------

    QOpenGLShaderProgram m_program;

    QOpenGLVertexArrayObject m_vao;

    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};


    GLuint m_texY  = 0;
    GLuint m_texUV = 0;   // NV12 için

    GLuint m_texU  = 0;   // YUV420P için
    GLuint m_texV  = 0;   // YUV420P için

    bool m_textureInitialized = false;

    int m_textureWidth  = 0;
    int m_textureHeight = 0;

    //----------------------------------------------------
    // Shader locations
    //----------------------------------------------------

    GLint m_locTexY = -1;
    GLint m_locTexU = -1;
    GLint m_locTexV = -1;

    GLint m_locPixelFormat;
    GLint m_locTexUV;


};

#endif
