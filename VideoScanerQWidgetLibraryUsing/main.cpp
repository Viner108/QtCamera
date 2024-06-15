#include <QApplication>
#include <videoscanerqwidgetlibrary.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(500, 500);
    w.setPalette(Qt::black);
    w.show();
    return a.exec();
}
