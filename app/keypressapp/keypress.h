#ifndef KEYPRESS_H
#define KEYPRESS_H
 
#include <QtGui/QWidget>
#include <QtGui/QtGui>
#include "fbupdate.h"
 
class KeyPress : public QWidget
{
    Q_OBJECT

public:
    KeyPress(QWidget *parent = 0);

signals:
    void UpdateWindow(QRect region);

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *event);
 
private:
    QLabel *myLabel;
    QVBoxLayout *mainLayout;
    FBUpdate *fbupdate;
};
 
#endif // KEYPRESS_H
