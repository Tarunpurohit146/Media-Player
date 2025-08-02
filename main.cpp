#include "mainwindow.h"

#include <QApplication>

void setStyle(MainWindow &w)
{
    QFile file(":/style/mainstyle.qss");
    if(file.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(file.readAll());
        w.setStyleSheet(style);
    } else {
        qDebug() << "Failed to add style sheet";
    }
}

int main(int argc, char *argv[])
{
    qputenv("QT_ENABLE_HIGHDPI_SCALING",QByteArray("0"));
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    w.setWindowIcon(QIcon(":/images/music.png"));
    setStyle(w);
    return a.exec();
}
