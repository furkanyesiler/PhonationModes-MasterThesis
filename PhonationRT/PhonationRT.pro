TEMPLATE = app
CONFIG += console c++11
QT += charts
QT += widgets
QT += core gui


SOURCES += \
    RtAudio.cpp \
    PhonationRTMain.cpp \
    PhonationRTWidget.cpp \
    PhonationRTProcess.cpp

HEADERS += \
    RtAudio.h \
    PhonationRTWidget.h \
    PhonationRTProcess.h

unix:!mac:!vxworks:!integrity:!haiku:LIBS += -lm

unix:!macx: LIBS += -lasound -lpthread

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lessentia

INCLUDEPATH += $$PWD/../../../../../usr/local/include

DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libessentia.a

unix:!macx: LIBS += -lfftw3 -lyaml -lavcodec -lavformat -lavutil -lavresample -lsamplerate -ltag -lfftw3f -lchromaprint
