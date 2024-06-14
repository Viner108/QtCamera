#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H
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
    CameraWidget(QWidget *parent = nullptr);

public slots:
    void captureFrame();
private:
    QCamera *camera;
    QVideoWidget *videoWidget;
    QMediaCaptureSession captureSession;
    QImageCapture *imageCapture;


};
#endif // CAMERAWIDGET_H
