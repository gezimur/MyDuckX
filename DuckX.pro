TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/src \
               $$PWD/include \
               $$PWD/thirdparty/zip \
               $$PWD/thirdparty/pugixml

SOURCES += \
            $$PWD/src/duckx.cpp \
            $$PWD/thirdparty/zip/zip.c \
            $$PWD/thirdparty/pugixml/pugixml.cpp

HEADERS += \
    $$PWD/include/constants.hpp \
    $$PWD/include/duckx.hpp \
    $$PWD/include/duckxiterator.hpp \
    $$PWD/thirdparty/zip/miniz.h \
    $$PWD/thirdparty/zip/zip.h \
    $$PWD/thirdparty/pugixml/pugiconfig.hpp \
    $$PWD/thirdparty/pugixml/pugixml.hpp


DESTDIR += $$PWD/lib
TARGET = DuckX_lib
