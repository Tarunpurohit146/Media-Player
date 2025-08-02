#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileCheck();
    initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileCheck(){
    filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/config.json";
    QFile file(filePath);
    if(!file.exists())
    {
        if (file.open(QIODevice::WriteOnly)) {
            file.close();
            qDebug() << "File created successfully at:" << filePath;
        } else {
            qDebug() << "Failed to create file.";
        }
    } else {
        qDebug() << "File already exists at:" << filePath;
    }

}

void MainWindow::initialize(){
    dashboard  = new Dashboard(this);
    dashboard->setObjectName("dashboard");
    dashboard->setConfigPath(filePath);
    setCentralWidget(dashboard);
    connect(dashboard, &Dashboard::videoPath, this, &MainWindow::setVideoWidget);
}

void MainWindow::setVideoWidget(QString filePath, bool isAudio){
    disconnect(dashboard, nullptr, this, nullptr);
    dashboard->deleteLater();
    dashboard = nullptr;

    qDebug() << "is audio:" << isAudio;
    video = new VideoWidget(this);
    if(isAudio){
        video->setUpAudioWidget();
    } else {
        video->setUpVideoWidget();
    }
    video->videoPath(filePath);
    setCentralWidget(video);
    connect(video, &VideoWidget::resetParent, this, [=](){
        video->setParent(this);
        setCentralWidget(video);
    });
    connect(video, &VideoWidget::closeVideoWidget, this, &MainWindow::closeVideoWidget);
}

void MainWindow::closeVideoWidget()
{
    video->deleteLater();
    initialize();
}
