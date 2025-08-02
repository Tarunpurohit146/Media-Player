#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFile>
#include <QStandardPaths>
#include "dashboard.h"
#include "videowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setVideoWidget(QString path, bool isAudio);

    void closeVideoWidget();
private:
    Ui::MainWindow *ui;
    QString filePath;
    Dashboard *dashboard;
    VideoWidget *video;

    void initialize();
    void fileCheck();
};
#endif // MAINWINDOW_H
