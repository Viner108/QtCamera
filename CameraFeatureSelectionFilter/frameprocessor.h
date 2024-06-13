#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QObject>
#include <QVideoFrame>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QThread>

class FrameProcessor : public QObject, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit FrameProcessor(QObject *parent = nullptr);
    ~FrameProcessor();
    void processFrame(const QVideoFrame &frame);

signals:
    void frameProcessed(const QImage &image);

public slots:
    void initializeOpenGL();

private:
    bool initialized;
    QOpenGLShaderProgram *program;
    GLuint VAO, VBO, EBO;
    QOpenGLContext *context;
    QOffscreenSurface *surface;
    float count = 0;
    int k = 1;
};

class FrameProcessorThread : public QThread {
    Q_OBJECT
public:
    explicit FrameProcessorThread(QObject *parent = nullptr) : QThread(parent), processor(nullptr) {}

    FrameProcessor* processor;
    void run() override {
        processor = new FrameProcessor();
        exec();
    }
};

#endif // FRAMEPROCESSOR_H
