#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QVBoxLayout>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

    void animation(QRect parent);
private:

};

#endif // MENUWIDGET_H
