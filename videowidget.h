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
#include <QPointer>
#include <QTimer>
#include <QKeyEvent>

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

    void keyPressEvent(QKeyEvent *event) override;
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
    QPointer<QGraphicsScene> scene;
    QPointer<QGraphicsView> view;
    QPointer<QGraphicsVideoItem> videoItem;
    QPointer<QPushButton> skipForwardButton;
    QPointer<QPushButton> skipBackButton;
    QPointer<QGraphicsProxyWidget> proxyWidget;

    QPushButton *playPauseButton;

    QVBoxLayout *mainLayout;

    QPushButton *volumeButton;

    QString filePath;

    QSlider *progressBar;

    QPointer<QMediaPlayer> player;
    QPointer<QAudioOutput> audio;

    QPointer<QWidget> volume;

    QPointer<QWidget> baseWidget;
    QPointer<QWidget> statusBar;

    void setUpStatusBar();
    void cleanUp();
    void closeVolumeWidget();
    void setUpPlayer();
};

#endif // VIDEOWIDGET_H
