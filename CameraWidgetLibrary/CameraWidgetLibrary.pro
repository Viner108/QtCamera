QT       += core gui multimedia multimediawidgets

TEMPLATE = lib
CONFIG +=c++17 dll
TARGET   = mydll
DEFINES += CAMERAWIDGETLIBRARY_LIBRARY

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camerawidgetlibrary.cpp

HEADERS += \
    camerawidgetlibrary.h

VERSION = 1.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
