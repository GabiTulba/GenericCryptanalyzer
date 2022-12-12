TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        abstractbitshiftbox.cpp \
        abstractbox.cpp \
        cipheranalyzer.cpp \
        ebox.cpp \
        identitybox.cpp \
        main.cpp \
        pbox.cpp \
        roundfunction.cpp \
        sbox.cpp

HEADERS += \
    abstractbitshiftbox.h \
    abstractbox.h \
    cipheranalyzer.h \
    ebox.h \
    identitybox.h \
    pbox.h \
    roundfunction.h \
    sbox.h
