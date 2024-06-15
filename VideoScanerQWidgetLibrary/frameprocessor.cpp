#include "frameprocessor.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <cmath>
#include <QDebug>
#include <QFile>
#include <QSurfaceFormat>
#include <QPainter>

FrameProcessor::FrameProcessor(QObject *parent) : QObject(parent), initialized(false), context(nullptr), surface(nullptr) {}

FrameProcessor::~FrameProcessor() {
    delete surface;
    delete context;
}

void FrameProcessor::initializeOpenGL() {
    if (!context) {
        context = new QOpenGLContext(this);
        QSurfaceFormat format;
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
        context->setFormat(format);
        context->create();

        surface = new QOffscreenSurface();
        surface->setFormat(context->format());
        surface->create();
    }

    context->makeCurrent(surface);
    initializeOpenGLFunctions();

    // Загрузка шейдеров из ресурсов
    QFile vertexFile(":/qresource/vertex_shader.glsl");
    QFile fragmentFile(":/qresource/sobel_fragment_shader.glsl");


    if (!vertexFile.open(QIODevice::ReadOnly) || !fragmentFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to load shader files";
        return;
    }

    QByteArray vertexShaderSource = vertexFile.readAll();
    QByteArray fragmentShaderSource = fragmentFile.readAll();

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    program->link();

    GLfloat vertices[] = {
        // positions    // texture coords
        1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initialized = true;
}

void FrameProcessor::processFrame(const QVideoFrame &frame) {
    if (!initialized) {
        initializeOpenGL();
    }

    context->makeCurrent(surface);

    QVideoFrame cloneFrame(frame);
    if (!cloneFrame.map(QVideoFrame::ReadOnly)) {
        qWarning() << "Failed to map frame";
        return;
    }

    QImage image = cloneFrame.toImage();
    cloneFrame.unmap();

    if (image.isNull()) {
        qWarning() << "Failed to convert frame to image";
        return;
    }

    // Переворачиваем изображение по вертикали
    //image = image.mirrored(false, true);

    // Координаты и размеры для вырезания полоски
    int x = count; // Начальная координата x для вырезания полоски (можно изменить)
    int y = 0; // Начальная координата y
    int stripWidth = 100; // Ширина полоски
    int height2 = image.height(); // Высота полоски будет равна высоте исходного изображения

    // Вырезаем полоску из исходного изображения
    QImage image2 = image.copy(x, y, stripWidth, height2);


    QOpenGLTexture texture(QOpenGLTexture::Target2D);
    texture.setData(image2);

    int width = image2.width();
    int height = image2.height();

    QOpenGLFramebufferObject fbo(width, height, QOpenGLFramebufferObject::CombinedDepthStencil);

    fbo.bind();
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    program->bind();
    glBindVertexArray(VAO);

    texture.bind();
    program->setUniformValue("image", 0);
    program->setUniformValue("width", float(width));
    program->setUniformValue("height", float(height));
    if(k % 2 == 0){
        program->setUniformValue("dir", float(1));
    }else{
        program->setUniformValue("dir", float(2));
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    texture.release();
    glBindVertexArray(0);
    program->release();
    fbo.release();

    QImage result = fbo.toImage();
    QPainter painter2(&image);

    // Накладываем image2 на image
    painter2.drawImage(x, y, result);

    // Заканчиваем рисование
    painter2.end();
    if(count > image.width()+ stripWidth || count < 0 - stripWidth){
        k+=1;
    }
    if(k % 2 == 0){
        count+=10;
    }else {
        count-=10;
    }
    emit frameProcessed(image);
}
