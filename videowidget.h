#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QSlider>
#include <QLabel>
#include <QProgressBar>
#include <QGraphicsProxyWidget>
#include <QAudioOutput>
#include <QToolTip>
#include <QTimer>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

    void setUpAudioWidget();
    void setUpVideoWidget();
    void videoPath(QString path);
public slots:
    void playVideo();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void resetParent();
    void closeVideoWidget();

private slots:
    void onPlayPauseButton();
    void onSkipBackButton();
    void onskipForwardButton();
    void onVolumeButton();
    void videoPosition(qint64 position);
    void playerStatus(QMediaPlayer::MediaStatus status);
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsVideoItem *videoItem;
    QGraphicsProxyWidget *proxyWidget;

    QPushButton *playPauseButton;

    QVBoxLayout *mainLayout;

    QPushButton *volumeButton;

    QString filePath;

    QSlider *progressBar;

    QMediaPlayer *player;
    QAudioOutput *audio;

    QWidget *volume;

    QWidget *baseWidget;
    QWidget *statusBar;

    void setUpStatusBar();
    void cleanUp();
    void closeVolumeWidget();
    void setUpPlayer();
};

#endif // VIDEOWIDGET_H
