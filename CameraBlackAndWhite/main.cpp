#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QVideoWidget>
#include <QImage>
#include <QLabel>
#include <QMediaDevices>

class CameraWidget : public QWidget {
    Q_OBJECT

public:
    CameraWidget(QWidget *parent = nullptr) : QWidget(parent), frameCount(0), processingEnabled(false) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Создаем виджет для отображения видео с камеры
        videoWidget = new QVideoWidget(this);
        layout->addWidget(videoWidget);

        // Метка для отображения обработанных кадров
        processedFrameLabel = new QLabel(this);
        processedFrameLabel->setMinimumSize(640, 480);  // Установим минимальный размер для метки
        layout->addWidget(processedFrameLabel);

        // Устанавливаем камеру
        camera = new QCamera(QMediaDevices::defaultVideoInput(), this);
        captureSession.setCamera(camera);
        captureSession.setVideoOutput(videoWidget);

        // Добавляем видео sink для обработки кадров
        videoSink = new QVideoSink(this);
        captureSession.setVideoOutput(videoSink);

        // Кнопка для запуска обработки видео
        QPushButton *startButton = new QPushButton("Start Processing", this);
        layout->addWidget(startButton);

        // Соединяем кнопку с функцией начала обработки
        connect(startButton, &QPushButton::clicked, this, &CameraWidget::startProcessing);

        // Обработка каждого кадра
        connect(videoSink, &QVideoSink::videoFrameChanged, this, &CameraWidget::processFrame);

        // Запуск камеры
        camera->start();
    }

public slots:
    void startProcessing() {
        processingEnabled = true;
    }

    void processFrame(const QVideoFrame &frame) {
        if (!processingEnabled) return;

        frameCount++;

        QVideoFrame cloneFrame(frame);
        if (!cloneFrame.isValid()) return;

        QImage img = cloneFrame.toImage();
        if (img.isNull()) {
            return;
        }

        if (frameCount % 3 == 0) {
            // Применение черно-белого фильтра к каждому третьему кадру
            img = img.convertToFormat(QImage::Format_Grayscale8);
        }

        // Отображение обработанного кадра
        processedFrameLabel->setPixmap(QPixmap::fromImage(img).scaled(processedFrameLabel->size(), Qt::KeepAspectRatio));
    }

private:
    QCamera *camera;
    QVideoWidget *videoWidget;
    QLabel *processedFrameLabel;
    QMediaCaptureSession captureSession;
    QVideoSink *videoSink;
    int frameCount;
    bool processingEnabled;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CameraWidget cameraWidget;
    cameraWidget.show();

    return app.exec();
}

#include "main.moc"
