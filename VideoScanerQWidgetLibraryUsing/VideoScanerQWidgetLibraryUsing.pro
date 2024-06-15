QT += core gui multimedia multimediawidgets widgets opengl

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Путь к заголовочным файлам библиотеки
INCLUDEPATH += D:\work\projects\qt\QtCamera\VideoScanerQWidgetLibrary

# Путь к скомпилированной библиотеке и ее имя
LIBS += -LD:\work\projects\qt\QtCamera\VideoScanerQWidgetLibrary\build\Desktop_Qt_6_7_1_MSVC2019_64bit-Debug\debug -lmydll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
