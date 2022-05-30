QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(3rdparty/gifimage/qtgifimage.pri)

CONFIG += c++17
#LIBS += -lglu32 -lopengl32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += HOME_DIR=\\\"$(HOME)\\\"

SOURCES += \
    3d.c \
    3rdParty/giflib/dgif_lib.c \
    3rdParty/giflib/egif_lib.c \
    3rdParty/giflib/gif_err.c \
    3rdParty/giflib/gif_font.c \
    3rdParty/giflib/gif_hash.c \
    3rdParty/giflib/gifalloc.c \
    3rdParty/giflib/quantize.c \
    main.cpp \
    widget.cpp

HEADERS += \
    3d.h \
    3rdParty/giflib/gif_hash.h \
    3rdParty/giflib/gif_lib.h \
    3rdParty/giflib/gif_lib_private.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    3rdParty/giflib.pri \
    3rdParty/giflib/AUTHORS \
    3rdParty/giflib/COPYING \
    3rdParty/giflib/README

RESOURCES +=
