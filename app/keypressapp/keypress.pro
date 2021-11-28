TEMPLATE = app
TARGET = keypress.app
DEPENDPATH += .
INCLUDEPATH += .

HEADERS       = keypress.h \
                digma_hw.h
SOURCES       = keypress.cpp \
                main.cpp \
                digma_hw.c

#QMAKE_RPATHDIR += /usr/lib/boeye

QMAKE_CXXFLAGS += -Wno-psabi # skip warning "the mangling of 'va_list' has changed in GCC 4.4"
