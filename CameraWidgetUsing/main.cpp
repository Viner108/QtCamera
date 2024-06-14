#include <QApplication>
#include <camerawidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CameraWidget cameraWidget;
    cameraWidget.resize(600, 600);
    cameraWidget.show();
    return a.exec();
}
