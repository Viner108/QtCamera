#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QVideoFrame>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include <QFile>
#include <QPushButton>
#include <QMediaDevices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    camera(new QCamera(QMediaDevices::defaultVideoInput(), this)),
    videoWidget(new QVideoWidget(this)),
    captureSession(new QMediaCaptureSession),
    videoSink(new QVideoSink(this)),
    label(new QLabel(this)),
    processingEnabled(false) {

    ui->setupUi(this);

    // Создаем вертикальный макет и добавляем videoWidget и кнопку
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(videoWidget);

    // Метка для отображения обработанных кадров
    label = new QLabel(this);
    label->setMinimumSize(640, 480);  // Установим минимальный размер для метки
    layout->addWidget(label);

    // Создаем центральный виджет и устанавливаем на него макет
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    captureSession->setCamera(camera);
    captureSession->setVideoOutput(videoWidget);
    captureSession->setVideoOutput(videoSink);

    saveButton = new QPushButton("Save Image", this);
    layout->addWidget(saveButton);

    // Кнопка для запуска обработки видео
    startButton = new QPushButton("Start Processing", this);
    layout->addWidget(startButton);

    // Соединяем кнопку с функцией начала обработки
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startProcessing);

    connect(videoSink, &QVideoSink::videoFrameChanged, this, &MainWindow::processFrame);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage); // Подключение кнопки

    camera->start();

    qDebug() << "Camera started";

    // Создаем фоновый поток для обработки видеокадров
    processingThread = new FrameProcessorThread(this);
    processingThread->start();

    // Подождем, пока поток не будет готов
    QMetaObject::invokeMethod(this, [this]() {
            connect(this, &MainWindow::processFrameInBackground, processingThread->processor, &FrameProcessor::processFrame);
            connect(processingThread->processor, &FrameProcessor::frameProcessed, this, &MainWindow::onFrameProcessed);
        }, Qt::QueuedConnection);
}

MainWindow::~MainWindow() {
    processingThread->quit();
    processingThread->wait();
    delete ui;
}

void MainWindow::startProcessing() {
    processingEnabled = true;
}

void MainWindow::processFrame(const QVideoFrame &frame) {
    if (!processingEnabled) {
        return;
    }

    emit processFrameInBackground(frame);
}

void MainWindow::onFrameProcessed(const QImage &image) {
    edgeImage = image;
    label->setPixmap(QPixmap::fromImage(edgeImage).scaled(label->size(), Qt::KeepAspectRatio));
}

void MainWindow::saveImage() {
    // Сохранение изображения в файл при нажатии кнопки
    if (!edgeImage.isNull()) {
        QFile file("/mnt/data/edge_image.png");
        if (file.open(QIODevice::WriteOnly)) {
            if (!edgeImage.save(&file, "PNG")) {
                qWarning() << "Failed to save image";
            } else {
                qDebug() << "Image saved successfully";
            }
            file.close();
        } else {
            qWarning() << "Failed to open file for writing";
        }
    } else {
        qWarning() << "No image to save";
    }
}
