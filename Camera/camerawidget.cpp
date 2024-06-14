#include "camerawidget.h"

CameraWidget::CameraWidget(QWidget *parent) : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Создаем виджет для отображения видео с камеры
    videoWidget = new QVideoWidget(this);
    layout->addWidget(videoWidget);

    // Устанавливаем камеру
    camera = new QCamera(QMediaDevices::defaultVideoInput(), this);
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(videoWidget);


    // Захват изображения
    imageCapture = new QImageCapture;
    captureSession.setImageCapture(imageCapture);

    // Кнопка для захвата кадра
    QPushButton *captureButton = new QPushButton("Capture Frame", this);
    layout->addWidget(captureButton);

    // Соединяем кнопку с функцией захвата кадра
    connect(captureButton, &QPushButton::clicked, this, &CameraWidget::captureFrame);

    // Запуск камеры
    camera->start();
}

void CameraWidget :: captureFrame() {
    // Захват кадра
    imageCapture->captureToFile("captured_frame.jpg");
}
