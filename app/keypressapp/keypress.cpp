#include "keypress.h"
#include <QApplication>
#include <QKeyEvent>


KeyPress::KeyPress(QWidget *parent) :
    QWidget(parent)
{
    myLabel = new QLabel("LABEL");
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(myLabel);
    setLayout(mainLayout);

    this->fbupdate = new FBUpdate();
    fbupdate->Init();
    connect(this, SIGNAL(UpdateWindow(QRect)), this->fbupdate,SLOT(Update(QRect)));
}

void KeyPress::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        myLabel->setText("You pressed BACK");
        break;
    case Qt::Key_Return:
        myLabel->setText("You pressed OK");
        break;
    case Qt::Key_F8:
        QApplication::exit(); //"HOME" properly quits application
        return;
    case Qt::Key_F9:
        myLabel->setText("You pressed MENU");
        break;
    case Qt::Key_F5:
// Otherwise, given present refresh issues, you'll never see any other key name.
//        myLabel->setText("You pressed REFRESH");
        break;
    case Qt::Key_Pause:
        myLabel->setText("You pressed POWER BUTTON");
        break;
    case Qt::Key_PageUp:
        myLabel->setText("You pressed PAGE UP");
        break;
    case Qt::Key_PageDown:
        myLabel->setText("You pressed PAGE DOWN");
        break;
    case Qt::Key_Up:
        myLabel->setText("You pressed UP ARROW");
        break;
    default:
        myLabel->setText("You pressed " + QKeySequence(event->key()).toString());
    }
}
 
void KeyPress::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        myLabel->setText("You released BACK");
        break;
    case Qt::Key_Return:
        myLabel->setText("You released OK");
        break;
    case Qt::Key_F9:
        myLabel->setText("You released MENU");
        break;
    case Qt::Key_F5:
// Otherwise, given present refresh issues, you'll never see any other key name.
//        myLabel->setText("You released REFRESH");
        break;
    case Qt::Key_Pause:
        myLabel->setText("You released POWER BUTTON");
        break;
    case Qt::Key_PageUp:
        myLabel->setText("You released PAGE UP");
        break;
    case Qt::Key_PageDown:
        myLabel->setText("You released PAGE DOWN");
        break;
    case Qt::Key_Up:
        myLabel->setText("You released UP ARROW");
        break;
    default:
        myLabel->setText("You released " + QKeySequence(event->key()).toString());
    }
}

void KeyPress::paintEvent(QPaintEvent *event)
{
    QRect region = event->rect();
    emit(this->UpdateWindow(region));
}
