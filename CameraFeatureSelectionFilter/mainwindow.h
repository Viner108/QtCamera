#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
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
    Ui::MainWindow *ui;
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

#endif // MAINWINDOW_H
