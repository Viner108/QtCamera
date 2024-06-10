#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QObject>
#include <QImage>
#include <QVideoFrame>

class FrameProcessor : public QObject {
    Q_OBJECT

public:
    explicit FrameProcessor(QObject *parent = nullptr);
    void processFrame(const QVideoFrame &frame);

signals:
    void frameProcessed(const QImage &image);

private:
    QImage edgeImage;
};

#endif // FRAMEPROCESSOR_H
