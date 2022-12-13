TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        abstractbitshiftbox.cpp \
        abstractbox.cpp \
        cipheranalyzer.cpp \
        ebox.cpp \
        helpers.cpp \
        identitybox.cpp \
        main.cpp \
        pbox.cpp \
        roundfunction.cpp \
        sbox.cpp \
        xorbox.cpp

HEADERS += \
    abstractbitshiftbox.h \
    abstractbox.h \
    cipheranalyzer.h \
    ebox.h \
    helpers.h \
    identitybox.h \
    pbox.h \
    roundfunction.h \
    sbox.h \
    xorbox.h
