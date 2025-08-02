#include "menuwidget.h"

#include <QPushButton>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QPushButton *cilckMe = new QPushButton("click me");
    mainLayout->addWidget(cilckMe);
    this->setLayout(mainLayout);
}

void MenuWidget::animation(QRect parent){
    //QPropertyAnimation *animationPos = new QPropertyAnimation(this, "pos", this);
    //animationPos->setDuration(1000);
    //animationPos->setStartValue(QPoint(0, 0));
    //animationPos->setEndValue(QPoint(parent.width() * 0.5, parent.height() * 0.9));
    //qDebug() << "pos:" << parent;
    //animationPos->start();

    QPropertyAnimation *animationSize = new QPropertyAnimation(this, "size");
    animationSize->setDuration(1000);
    animationSize->setStartValue(QSize(0, parent.height() * 0.9));
    animationSize->setEndValue(QSize(parent.width() * 0.5, parent.height() * 0.9));
    animationSize->start(QAbstractAnimation::DeleteWhenStopped);
    qDebug() << parent;
}
