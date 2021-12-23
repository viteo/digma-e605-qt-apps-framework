TEMPLATE = app
TARGET = firstapp.app
DEPENDPATH += .
INCLUDEPATH += .

HEADERS       = digitalclock.h \
                fbupdate.h
SOURCES       = digitalclock.cpp \
                main.cpp \
                fbupdate.cpp

QMAKE_CXXFLAGS += -Wno-psabi # skip warning "the mangling of 'va_list' has changed in GCC 4.4"
