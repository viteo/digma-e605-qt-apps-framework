#include <QApplication>
#include "keypress.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KeyPress keyPress;
    keyPress.show();
    return app.exec();
}
