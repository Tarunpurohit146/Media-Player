#include "videowidget.h"
#include <QGraphicsWidget>

VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent)
    , videoItem(nullptr)
    , proxyWidget(nullptr)
    , player(nullptr)
    , baseWidget(nullptr)
    , statusBar(nullptr)
    , volume(nullptr)
    , progressBar(nullptr)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);
    this->installEventFilter(this);
}

VideoWidget::~VideoWidget(){
    if(player)
    {
        player->stop();
        player->deleteLater();
        audio->deleteLater();
    }
}

void VideoWidget::setUpPlayer()
{
    player = new QMediaPlayer;
    audio = new QAudioOutput;
    player->setAudioOutput(audio);
    audio->setVolume(0.5);
    connect(player, &QMediaPlayer::positionChanged, this, &VideoWidget::videoPosition);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &VideoWidget::playerStatus);
    connect(player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString){
        qDebug() << "Playback error:" << error << errorString;
    });
}

void VideoWidget::setUpAudioWidget()
{

    QWidget *audioWidget = new QWidget(this);
    QVBoxLayout *audioWidgetLayout = new QVBoxLayout(audioWidget);
    audioWidget->setObjectName("audioWidget");
    QLabel *icon = new QLabel(audioWidget);
    icon->setStyleSheet("QLabel { "
                        "border-radius: "+QString::number(icon->width() / 2)+"px;"
                        "background-color: rgba(120, 120, 120, 150);"
                        "}");
    QPixmap pixmap = QPixmap(":/images/music.png");
    pixmap = pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    icon->setPixmap(pixmap);
    icon->setAlignment(Qt::AlignCenter);
    audioWidgetLayout->addWidget(icon);
    audioWidget->setLayout(audioWidgetLayout);

    mainLayout->addWidget(audioWidget);
    setUpStatusBar();
    setUpPlayer();
}

void VideoWidget::setUpVideoWidget(){

    baseWidget = new QWidget(this);

    view = new QGraphicsView(baseWidget);
    scene = new QGraphicsScene(view);
    videoItem = new QGraphicsVideoItem();

    setUpPlayer();

    scene->addItem(videoItem);

    player->setVideoOutput(videoItem);

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *baseLayout = new QVBoxLayout(baseWidget);

    baseWidget->setStyleSheet("background-color: black;"
                              "border-radius: 5px;");

    baseLayout->setAlignment(Qt::AlignCenter);
    baseLayout->setContentsMargins(0,0,0,0);
    baseLayout->addWidget(view);
    baseWidget->setLayout(baseLayout);
    mainLayout->addWidget(baseWidget);
    setUpStatusBar();

}

void VideoWidget::playerStatus(QMediaPlayer::MediaStatus status)
{

    if(status == QMediaPlayer::BufferedMedia)
    {
        qDebug() << "setting progressBar" << progressBar;
        progressBar->setRange(0, player->duration());
    }
}

void VideoWidget::videoPosition(qint64 position)
{
    if(progressBar)
    {
        qDebug() << "position";
        progressBar->setValue(position);
    }
}

void VideoWidget::videoPath(QString path)
{
    filePath = path;
    qDebug() << "file path:" << path;

    player->setSource(QUrl(path));
    player->play();
}

void VideoWidget::setUpStatusBar() {

    qDebug() << "setting status bar";
    statusBar = new QWidget(this);
    statusBar->setObjectName("statusBar");
    qDebug() << "declare done";

    QHBoxLayout *hLayout = new QHBoxLayout(statusBar);
    QPushButton *stopVideo = new QPushButton(statusBar);
    stopVideo->setIcon(QIcon(":/images/stop-button.png"));
    stopVideo->setIconSize(QSize(40, 40));
    progressBar = new QSlider(Qt::Horizontal, statusBar);
    progressBar->setObjectName("progressBar");

    playPauseButton = new QPushButton(statusBar);
    playPauseButton->setIcon(QIcon(":/images/pause.png"));
    playPauseButton->setIconSize(QSize(35,35));
    QPushButton *skipBackButton = new QPushButton(statusBar);
    skipBackButton->setIcon(QIcon(":/images/rewind.png"));
    skipBackButton->setIconSize(QSize(35, 35));
    QPushButton *skipForwardButton = new QPushButton(statusBar);
    skipForwardButton->setIcon(QIcon(":/images/fast-forward.png"));
    skipForwardButton->setIconSize(QSize(35, 35));
    volumeButton = new QPushButton(statusBar);
    volumeButton->setIcon(QIcon(":/images/volume.png"));
    volumeButton->setIconSize(QSize(35,35));

    qDebug() << "second declare done";

    hLayout->addWidget(stopVideo);
    hLayout->addWidget(playPauseButton);
    hLayout->addWidget(skipBackButton);
    hLayout->addWidget(progressBar);
    hLayout->addWidget(skipForwardButton);
    hLayout->addWidget(volumeButton);
    qDebug() << "assign done";

    statusBar->setLayout(hLayout);

    qDebug() << "size declare done";

    qDebug() << "proxy widget done";

    statusBar->hide();

    qDebug() << "finished setting menu";
    connect(playPauseButton, &QPushButton::clicked, this, &VideoWidget::onPlayPauseButton);
    connect(skipBackButton, &QPushButton::clicked, this, &VideoWidget::onSkipBackButton);
    connect(skipForwardButton, &QPushButton::clicked, this, &VideoWidget::onskipForwardButton);
    connect(volumeButton, &QPushButton::clicked, this, &VideoWidget::onVolumeButton);
    connect(progressBar, &QSlider::sliderMoved, this, [=](int position){
        player->setPosition(position);
    });
    connect(progressBar, &QSlider::sliderPressed, this, [=](){
        player->setPosition(progressBar->value());
    });
    connect(stopVideo, &QPushButton::clicked, this, [=](){
        emit closeVideoWidget();
    });
}

void VideoWidget::onSkipBackButton()
{
    int duration = player->duration();
    int position = player->position();
    int newPosition = position - 10000;
    qDebug() << "rewind :" << duration << position << newPosition;
    if(newPosition<=duration)
    {
        player->setPosition(newPosition);
    }
}

void VideoWidget::onskipForwardButton()
{
    int duration = player->duration();
    int position = player->position();
    int newPosition = position + 10000;
    qDebug() << "forward :" << duration << position << newPosition;
     player->setPosition(newPosition);
    if(newPosition<=duration)
    {
        player->setPosition(newPosition);
    }
}

void VideoWidget::onPlayPauseButton()
{
    if(player->isPlaying()){
        player->pause();
        playPauseButton->setIcon(QIcon(":/images/play.png"));

    } else{
        player->play();

        playPauseButton->setIcon(QIcon(":/images/pause.png"));
    }
}

void VideoWidget::onVolumeButton()
{
    if(volume){
        closeVolumeWidget();
        return;
    }
    volume = new QWidget(this);
    volume->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QVBoxLayout *volumeLayout = new QVBoxLayout(volume);
    QLabel *hightVolume = new QLabel("100", volume);
    QSlider *slider = new QSlider(Qt::Vertical, volume);
    QLabel *lowVolume = new QLabel("0", volume);
    slider->setRange(0, 100);
    slider->setValue(50);
    volumeLayout->addWidget(hightVolume);
    volumeLayout->addWidget(slider);
    volumeLayout->addWidget(lowVolume);
    volume->setLayout(volumeLayout);
    QPoint globalPos = volumeButton->mapToGlobal(QPoint(0, volumeButton->height() - 200));
    volume->move(globalPos);
    connect(slider, &QSlider::sliderMoved, this, [=](int position)
            {
        qDebug() << "volume:" << position;
        audio->setVolume(position / 100.0);
        QToolTip::showText(QCursor::pos(), QString::number(position));

    });
    volume->show();
}

void VideoWidget::playVideo(){
    cleanUp();
}

bool VideoWidget::eventFilter(QObject *obj, QEvent *event){

    if(obj == this){
        if(event->type() == QEvent::Resize){
            QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
            if(statusBar->isVisible())
                statusBar->hide();

            closeVolumeWidget();

            if(videoItem){
                videoItem->setSize(resizeEvent->size());
            }
        }
        else if(event->type() == QEvent::MouseButtonPress)
        {
            if(statusBar->isVisible()){
                closeVolumeWidget();
                statusBar->hide();
            } else {
                statusBar->show();
                statusBar->setFixedHeight(this->height() * 0.15);
                statusBar->setFixedWidth(this->width() * 0.9);
                statusBar->move((this->width() - statusBar->width()) / 2, (this->height() - statusBar->height()) * 0.95);
            }
        }
        else if(event->type() == QEvent::MouseButtonDblClick)
        {
            if(statusBar->isVisible())
                statusBar->hide();

            closeVolumeWidget();
            if(!this->isFullScreen()) {
                this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
                this->showFullScreen();
            } else {
                this->setWindowFlags(Qt::Widget);
                emit resetParent();
                this->showNormal();
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void VideoWidget::cleanUp(){
    QLayoutItem *item;
    while((item = baseWidget->layout()->itemAt(0))){
        if(item->widget()){
            delete item->widget();
        }
    }
    delete item;
}

void VideoWidget::closeVolumeWidget()
{
    if(volume){
        delete volume;
        volume = nullptr;
    }
}
