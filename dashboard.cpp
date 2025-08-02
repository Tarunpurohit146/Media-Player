#include "dashboard.h"

Dashboard::Dashboard(QWidget *parent)
    : QWidget{parent}
    , isAudio(false)
    , screenRect(this->screen()->geometry())
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    mainLayout->setSpacing(0);

    audioExtensions << "flac" << "mp3" << "ogg" << "oga" << "aac"
                    << "wma" << "m4a" << "wav";
    extensions << "wmv" << "mov" << "mp4" << "mkv" << "avi" << audioExtensions;

}

Dashboard::~Dashboard()
{

}

void Dashboard::setConfigPath(QString path)
{
    qDebug() << "config:" << path;
    configPath = path;
    loadSavedPath();
    initialize();
}

void Dashboard::loadSavedPath()
{
    qDebug() << "file:" << configPath;
    QFile file(configPath);
    if(!file.open(QFile::ReadWrite))
    {
        qDebug() << "failed to open file";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(doc.isNull())
    {
        qDebug() << "not able to get json data";
        return;
    }
    qDebug() << "json Data:" << doc;
    saveFilePath = doc["savePath"].toArray();
    data.clear();
    data.squeeze();
}

void Dashboard::updateConfigFile(QString text)
{
    QFile file(configPath);
    qDebug() << "file text";

    QJsonObject obj;
    QJsonArray arr;

    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            file.close();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (!doc.isNull() && doc.isObject()) {
                obj = doc.object();
                arr = obj["savePath"].toArray();
            }
        }
    }

    if(!arr.contains(text)){
        arr.insert(0, text);
    }
    obj["savePath"] = arr;

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument newDoc(obj);
        file.write(newDoc.toJson(QJsonDocument::Indented));
        file.close();
    } else {
        qDebug() << "Error opening file for writing!";
    }
}

void Dashboard::onSelectedButtonClicked()
{
    QStringList patterns;
    for (const QString &ext : extensions)
        patterns << "*." + ext;

    QString filter = "Supported Media Files (" + patterns.join(' ') + ")";
    QString filePath = QFileDialog::getOpenFileName(this, "All File", "", filter);
    qDebug() << "File selected:" << filePath;
    QString extension = filePath.split(".").last();
    if(audioExtensions.contains(extension))
        isAudio = true;

    qDebug() << "isAudio File:" << isAudio;
    if(!filePath.isEmpty()){
        updateConfigFile(filePath);
        if(itemsContainer){
            mainLayout->removeWidget(itemsContainer);
            delete itemsContainer;
            loadSavedPath();
            setFilePath();
            emit videoPath(filePath, isAudio);
        }
    }
}

void  Dashboard::onSelectedNetworkClicked()
{

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Network");
    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);

    QHBoxLayout *labelLayout = new QHBoxLayout(dialog);
    QLabel *label = new QLabel("Enter Url", dialog);
    QLineEdit *lineEdit = new QLineEdit(dialog);

    QPushButton *submit = new QPushButton("Submit", dialog);

    labelLayout->addWidget(label);
    labelLayout->addWidget(lineEdit);

    dialogLayout->addLayout(labelLayout);
    dialogLayout->addWidget(submit);
    dialogLayout->setAlignment(Qt::AlignCenter);
    dialog->setLayout(dialogLayout);
    dialog->setContentsMargins(10, 10, 10, 10);
    dialog->setMinimumWidth(this->width() * 0.4);

    connect(submit, &QPushButton::clicked, this, [=](){
        QString text = lineEdit->text();
        QStringList splitText = text.split(".");
        QString extension = splitText.last();
        isAudio = audioExtensions.contains(extension) ? true : false;

        if(extensions.contains(extension))
        {
            updateConfigFile(lineEdit->text());
            if(itemsContainer){
                mainLayout->removeWidget(itemsContainer);
                delete itemsContainer;
                loadSavedPath();
                setFilePath();
                emit videoPath(text, isAudio);
                dialog->accept();
            }
        } else {
            QMessageBox::warning(this, "File Extenstion", "The file can't be played");
        }
    });
    dialog->exec();
}

void Dashboard::setButtons()
{
    QWidget *buttonContainer = new QWidget();
    buttonContainer->setObjectName("buttonContainer");

    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonContainer);
    QPushButton *selectLocalFiles = new QPushButton(buttonContainer);
    selectLocalFiles->setToolTip("Select File");
    QPushButton *selectNetwork = new QPushButton(buttonContainer);
    selectNetwork->setToolTip("Network");

    selectLocalFiles->setIcon(QIcon(":/images/file.png"));
    selectNetwork->setIcon(QIcon(":/images/network.png"));

    selectNetwork->setCursor(Qt::PointingHandCursor);
    selectLocalFiles->setCursor(Qt::PointingHandCursor);

    buttonLayout->addWidget(selectLocalFiles);
    buttonLayout->addWidget(selectNetwork);
    buttonLayout->setAlignment(Qt::AlignCenter);

    buttonContainer->setLayout(buttonLayout);

    buttonContainer->setMaximumWidth(screenRect.width() * 0.3);

    selectLocalFiles->setMaximumSize(buttonContainer->size() * 0.4);
    selectLocalFiles->setIconSize(buttonContainer->size() * 0.3);

    selectNetwork->setMaximumSize(buttonContainer->size() * 0.4);
    selectNetwork->setIconSize(buttonContainer->size() * 0.3);

    buttonLayout->setSpacing(buttonContainer->height() * 0.4);

    mainLayout->addWidget(buttonContainer);
    buttonContainer->setParent(this);

    connect(selectLocalFiles,  &QPushButton::clicked, this, &Dashboard::onSelectedButtonClicked);
    connect(selectNetwork, &QPushButton::clicked, this, &Dashboard::onSelectedNetworkClicked);
}

void Dashboard::selectedFile(const QModelIndex &current, const QModelIndex &previous)
{

    QStringList selectedText = model->data(current, Qt::DisplayRole).toString().split("\n");
    QString filePath = selectedText.last();
    qDebug() << "selected:" <<  selectedText.last();
    QString extension = selectedText.last().split(".").last();
    isAudio = audioExtensions.contains(extension) ? true : false;
    emit videoPath(filePath, isAudio);
}

void Dashboard::setFilePath()
{
    itemsContainer = new QWidget(this);
    itemsContainer->setObjectName("itemsContainer");
    QVBoxLayout *itemsContainerLayout = new QVBoxLayout(itemsContainer);

    if(!saveFilePath.isEmpty()){
        QListView *listView = new QListView(itemsContainer);
        listView->setObjectName("listView");

        model = new QStandardItemModel(listView);

        for (int i=0; i<saveFilePath.size(); i++) {
            QString text = saveFilePath.at(i).toString();
            qDebug() << "text:" << text;
            QStringList textList = text.split("/");
            QStandardItem *item = new QStandardItem(textList.at(textList.size() -1)+"\n"+text);
            model->appendRow(item);
        }
        listView->setModel(model);
        listView->setSpacing(10);
        itemsContainerLayout->addWidget(listView);
        connect(listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &Dashboard::selectedFile);
    } else {
        QLabel *imageLabel = new QLabel(itemsContainer);
        imageLabel->setObjectName("imageLabel");
        QPixmap pixmap(":/images/empty-folder.png");
        pixmap = pixmap.scaled(this->screen()->geometry().size() * 0.4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(pixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        itemsContainerLayout->addWidget(imageLabel);
    }
    itemsContainer->setLayout(itemsContainerLayout);
    itemsContainer->setMaximumWidth(this->screen()->geometry().width() * 0.7);
    mainLayout->addWidget(itemsContainer);
}

void Dashboard::initialize()
{
    setButtons();
    setFilePath();
}
