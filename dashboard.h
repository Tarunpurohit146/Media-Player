#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QLabel>
#include <QMovie>
#include <QScreen>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QUrl>
#include <QMediaFormat>
#include <QLineEdit>
#include <QMessageBox>

class Dashboard : public QWidget
{
    Q_OBJECT
public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    void setConfigPath(QString path);
private slots:
    void onSelectedButtonClicked();
    void onSelectedNetworkClicked();
    void selectedFile(const QModelIndex &current, const QModelIndex &previous);

signals:
    void videoPath(QString path, bool isAudio);

private:
    void initialize();
    void loadSavedPath();

    QHBoxLayout *mainLayout;

    QStringList filePath;
    QString configPath;

    QStringList extensions;
    QStringList audioExtensions;

    bool isAudio;
    QStandardItemModel *model;

    QJsonArray saveFilePath;

    QRect screenRect;

    QMediaFormat format;


    void setButtons();
    void extracted(QStandardItemModel *&model);
    void setFilePath();
    void updateConfigFile(QString text);
    QWidget *itemsContainer;
};

#endif // DASHBOARD_H
