#include "frameprocessor.h"
#include <QImage>
#include <cmath>
#include <QDebug>

FrameProcessor::FrameProcessor(QObject *parent) : QObject(parent) {}

void FrameProcessor::processFrame(const QVideoFrame &frame) {
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

    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
    edgeImage = QImage(grayImage.size(), QImage::Format_Grayscale8);

    for (int y = 1; y < grayImage.height() - 1; ++y) {
        for (int x = 1; x < grayImage.width() - 1; ++x) {
            int gx = -grayImage.pixelColor(x - 1, y - 1).value() - 2 * grayImage.pixelColor(x - 1, y).value() - grayImage.pixelColor(x - 1, y + 1).value()
                     + grayImage.pixelColor(x + 1, y - 1).value() + 2 * grayImage.pixelColor(x + 1, y).value() + grayImage.pixelColor(x + 1, y + 1).value();
            int gy = -grayImage.pixelColor(x - 1, y - 1).value() - 2 * grayImage.pixelColor(x, y - 1).value() - grayImage.pixelColor(x + 1, y - 1).value()
                     + grayImage.pixelColor(x - 1, y + 1).value() + 2 * grayImage.pixelColor(x, y + 1).value() + grayImage.pixelColor(x + 1, y + 1).value();

            int g = qBound(0, int(std::sqrt(gx * gx + gy * gy)), 255);
            edgeImage.setPixel(x, y, qRgb(g, g, g));
        }
    }

    emit frameProcessed(edgeImage);
}
