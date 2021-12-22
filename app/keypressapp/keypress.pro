TEMPLATE = app
TARGET = keypress.app
DEPENDPATH += .
INCLUDEPATH += .

HEADERS       = keypress.h \
                fbupdate.h
SOURCES       = keypress.cpp \
                main.cpp \
                fbupdate.cpp

#QMAKE_RPATHDIR += /usr/lib/boeye

QMAKE_CXXFLAGS += -Wno-psabi # skip warning "the mangling of 'va_list' has changed in GCC 4.4"
