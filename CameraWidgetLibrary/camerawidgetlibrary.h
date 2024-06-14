#ifndef CAMERAWIDGETLIBRARY_H
#define CAMERAWIDGETLIBRARY_H
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QMediaDevices>
#include <QImageCapture>
#include <QLabel>

#include <QtCore/qglobal.h>

#if defined(CAMERAWIDGETLIBRARY_LIBRARY)
#  define CAMERAWIDGETLIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CAMERAWIDGETLIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

class CAMERAWIDGETLIBRARYSHARED_EXPORT CameraWidget : public QWidget {
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

#endif // CAMERAWIDGETLIBRARY_H
