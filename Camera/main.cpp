#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QMediaDevices>
#include <QImageCapture>
#include <QLabel>

class CameraWidget : public QWidget {
    Q_OBJECT

public:
    CameraWidget(QWidget *parent = nullptr) : QWidget(parent) {

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

public slots:
    void captureFrame() {
        // Захват кадра
        imageCapture->captureToFile("captured_frame.jpg");
    }

private:
    QCamera *camera;
    QVideoWidget *videoWidget;
    QMediaCaptureSession captureSession;
    QImageCapture *imageCapture;


};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CameraWidget cameraWidget;
    cameraWidget.resize(600, 600);
    cameraWidget.show();

    return app.exec();
}

// Включение сгенерированного файла мета-объекта
#include "main.moc"
