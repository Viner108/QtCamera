#include <QApplication>
#include <QWidget>
#include <camerawidget.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CameraWidget cameraWidget;
    cameraWidget.resize(600, 600);
    cameraWidget.show();

    return app.exec();
}

