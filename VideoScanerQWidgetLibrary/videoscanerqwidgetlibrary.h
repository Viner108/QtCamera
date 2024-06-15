#ifndef VIDEOSCANERQWIDGETLIBRARY_H
#define VIDEOSCANERQWIDGETLIBRARY_H

#include <QMainWindow>
#include <QCamera>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include "frameprocessor.h"

#include <QtCore/qglobal.h>

#if defined(VIDEOSCANERQWIDGETLIBRARY_LIBRARY)
#  define VIDEOSCANERQWIDGETLIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VIDEOSCANERQWIDGETLIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

class VIDEOSCANERQWIDGETLIBRARYSHARED_EXPORT MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void processFrameInBackground(const QVideoFrame &frame);
private slots:
    void startProcessing();
    void processFrame(const QVideoFrame &frame);
    void onFrameProcessed(const QImage &image);
    void saveImage();

private:
    QCamera *camera;
    QVideoWidget *videoWidget;
    QMediaCaptureSession *captureSession;
    QVideoSink *videoSink;
    QLabel *label;
    QImage edgeImage;
    QPushButton *saveButton;
    QPushButton *startButton;
    bool processingEnabled;
    FrameProcessorThread *processingThread;
};

#endif // VIDEOSCANERQWIDGETLIBRARY_H
