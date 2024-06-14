QT       += core gui multimedia multimediawidgets

CONFIG   += c++17 console
TEMPLATE = app
TARGET = myapp

# Путь к исходным файлам
SOURCES += \
    main.cpp

# Путь к заголовочным файлам библиотеки
INCLUDEPATH += D:/work/projects/qt/QtCamera/CameraWidgetLibrary

# Путь к скомпилированной библиотеке и ее имя
LIBS += -LD:/work/projects/qt/QtCamera/CameraWidgetLibrary/build/Desktop_Qt_6_7_1_MSVC2019_64bit-Debug/debug -lmydll1


