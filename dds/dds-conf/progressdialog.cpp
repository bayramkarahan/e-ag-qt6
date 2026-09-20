#include "progressdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include<QApplication>
#include<QScreen>
#include<QDesktopWidget>

ProgressDialog::ProgressDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("UlakShare Dosya Payşalım Paneli");
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    QSize screenSize = qApp->screens()[0]->size();
    h=screenSize.height()/15.1;
    w=h*2;
    int nfont=h/5;
    font=QString::number(nfont);
    int x = (screenGeometry.width()/2 - w*3);
    int y = (screenGeometry.height() - w*3) / 2;
    this->move(x, y);
    //qDebug()<<w<<h<<screenSize.height()/16;
    setStyleSheet(QString(R"(
    QDialog {
        background-color: #f5f7fb;
        color: #1f2a37;
        font-family: Arial;
        font-size: %1px;
    }

    /* BUTTONS */

    QToolButton {
        background-color: #ffffff;
        border: 1px solid #e5eaf2;
        padding: 5px 10px;
        border-radius: 8px;
        color: #1f2a37;
        /*font-weight:600;*/
        font-size: %1px;
    }

    QToolButton:hover {
        background-color: #f0f6ff;
        border: 1px solid #cfe2ff;
    }

    QToolButton:pressed {
        background-color: #e6f0ff;
    }

    /* PRIMARY BUTTON */
/*
    QToolButton#sendFileButton {
        background-color: #ffffff;
        color: white;
        border: none;
    }

    QToolButton#sendFileButton:hover {
        background-color: #256fd1;
    }*/

    /* TOTAL BAR */

    QProgressBar {
        border: none;
        border-radius: 6px;
        background-color: #e9eef5;
        text-align: center;
        color: #1f2a37;
        height: 10px;
        font-size:%1px;
    }

    QProgressBar::chunk {
        background-color: #2f80ed;
        border-radius: 6px;
    }

    QScrollArea {
        border: none;
        background: transparent;
    }

QLineEdit {
    background-color: #ffffff;
    border: 1px solid #e5eaf2;
    border-radius: 8px;
    padding: 2px 2px;
    color: #1f2a37;
    selection-background-color: #2f80ed;
    selection-color: white;
    font-size: %1px;
}

QLineEdit:hover {
    border: 1px solid #cfe2ff;
    background-color: #fafdff;
}

QLineEdit:focus {
    border: 1px solid #2f80ed;
    background-color: #ffffff;
font-size: %1px;
}

QLineEdit:disabled {
    background-color: #eef2f7;
    color: #8a94a6;
}
    )").arg(font));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6,6,6,6);
    mainLayout->setSpacing(4);

    // =====================================================
    // TOP BAR
    // =====================================================

    buttonWidget = new QWidget;
    QHBoxLayout *topBar = new QHBoxLayout(buttonWidget);
    topBar->setContentsMargins(0,0,0,0);
    topBar->setSpacing(0);

    pathWidget = new QWidget;
    //pathWidget->resize(w*6,h*0.5);
    QHBoxLayout *pathLayout = new QHBoxLayout(pathWidget);
    pathLayout->setContentsMargins(0,0,0,0);
    pathLayout->setSpacing(0);

    QLabel *filePathLabel=new QLabel("Dosya Konumu");
    filePathLabel->setFixedSize(w,h*0.5);
    filePathLabel->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");

    filePathLE = new QLineEdit();
    filePathLE->setFixedSize(w*4.5,h*0.5);
    filePathLE->setStyleSheet("font-size:"+QString::number(font.toInt()+2)+"px;");

    QToolButton *fileSelectButton=new QToolButton();
    fileSelectButton->setFixedSize(w/2,h*0.5);
    fileSelectButton->setAutoRaise(true);
    //fileSelectButton->setAutoFillBackground(true);
    fileSelectButton->setText(tr("..."));
    fileSelectButton->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");
    fileSelectButton->setIcon(QIcon(":/icons/file.svg"));
    fileSelectButton->setIconSize(QSize(w/2,h*0.7));
    fileSelectButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

    connect(fileSelectButton, &QToolButton::clicked, [=]() {

       // QString seatUser = helper.detectActiveUser();// 1) Aktif kullanıcıyı bul
        //qDebug()<<"Kullanıcı: "<<sessionUser;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Dosya Seç"),"/home/", tr("Files (*)"));
        filePathLE->setText(fileName);
        updatePath(fileName);

    });
    QToolButton *directorySelectButton=new QToolButton();
    directorySelectButton->setFixedSize(w/2,h*0.5);
    directorySelectButton->setAutoRaise(true);
    //directorySelectButton->setAutoFillBackground(true);
    directorySelectButton->setText(tr("..."));
    directorySelectButton->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");
    directorySelectButton->setIcon(QIcon(":/icons/directory.svg"));
    directorySelectButton->setIconSize(QSize(w/2,h*0.7));
    directorySelectButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

    connect(directorySelectButton, &QToolButton::clicked, [=]() {

       // QString seatUser = helper.detectActiveUser();// 1) Aktif kullanıcıyı bul
        //qDebug()<<"Kullanıcı: "<<sessionUser;
        QString dirPath = QFileDialog::getExistingDirectory(
                    this,
                    tr("Dizin Seç"),
                    "/home/"
                    );

        if(dirPath.isEmpty())
            return;

        filePathLE->setText(dirPath);

        updatePath(dirPath);

    });
    pathLayout->addWidget(filePathLabel);
    pathLayout->addWidget(filePathLE);
    pathLayout->addStretch();
    pathLayout->addWidget(fileSelectButton);
    pathLayout->addWidget(directorySelectButton);

    QToolButton *roleChangeButton= new QToolButton;
    roleChangeButton->setFixedSize(w*0.7,h);
    roleChangeButton->setIconSize(QSize(w*0.4,h*0.4));
    roleChangeButton->setStyleSheet("Text-align:center");
    roleChangeButton->setIcon(QIcon(":/icons/settings.svg"));
    roleChangeButton->setAutoRaise(true);
    roleChangeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    roleChangeButton->setText("Ayarlar");

    connect(roleChangeButton, &QPushButton::clicked, [=]() {
#ifdef Q_OS_LINUX
        QProcess p;
        p.start("pkexec", {"true"});
        p.waitForFinished();

        if(p.exitCode() == 0)
        {
            qDebug()<<"yetki:"<<p.exitCode();
                RoleDialog *dlg=new RoleDialog(w*1.5,h*1.2);
                if (dlg->exec() == QDialog::Accepted) {
                    qDebug() << "ulakshare rolü değiştirildi.";
                }
        }

#endif

    });

    discoveryButton = new QToolButton;
    discoveryButton->setText("Pc Ara");
    discoveryButton->setFixedSize(w*0.7,h);
    discoveryButton->setIcon(QIcon(":/icons/search.svg"));
    discoveryButton->setIconSize(QSize(w*0.4,h*0.4));
    discoveryButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(discoveryButton,
            &QToolButton::clicked,
            this,
            &ProgressDialog::onDiscoveryButtonClicked);

    sendFileButton = new QToolButton;
    sendFileButton->setObjectName("sendFileButton");
    sendFileButton->setText("Gönder");
    sendFileButton->setFixedSize(w*0.7,h);
    sendFileButton->setIcon(QIcon(":/icons/send.svg"));
    sendFileButton->setIconSize(QSize(w*0.4,h*0.4));
    sendFileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    /*connect(sendFileButton,
            &QToolButton::clicked,
            this,
            &ProgressDialog::onsendFileButtonClicked);
    */
    sendFileButton->setMenu(sendMenu());
    sendFileButton->setPopupMode(QToolButton::InstantPopup);


    selectAllButton = new QToolButton;
    selectAllButton->setText("Tümünü Seç");
    selectAllButton->setFixedSize(w*0.8,h);
    selectAllButton->setIcon(QIcon(":/icons/select.svg"));
    selectAllButton->setIconSize(QSize(w*0.4,h*0.4));
    selectAllButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(selectAllButton,
            &QToolButton::clicked,
            this,
            &ProgressDialog::selectAllButtonClicked);

    unSelectAllButton = new QToolButton;
    unSelectAllButton->setText("Seçimi Kaldır");
    unSelectAllButton->setFixedSize(w,h);
    unSelectAllButton->setIcon(QIcon(":/icons/unselect.svg"));
    unSelectAllButton->setIconSize(QSize(w*0.8,h*0.4));
    unSelectAllButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(unSelectAllButton,
            &QToolButton::clicked,
            this,
            &ProgressDialog::unSelectAllButtonClicked);

    getFileButton = new QToolButton;
    getFileButton->setText("Getir");
    getFileButton->setFixedSize(w*0.7,h);
    getFileButton->setIcon(QIcon(":/icons/getdesktop.svg"));
    getFileButton->setIconSize(QSize(w*0.5,h*0.4));
    getFileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(getFileButton,
            &QToolButton::clicked,
            this,
            &ProgressDialog::onGetFileButtonClicked);

    topBar->addWidget(roleChangeButton);
    topBar->addWidget(discoveryButton);
    topBar->addWidget(selectAllButton);
    topBar->addWidget(unSelectAllButton);
    topBar->addStretch();
    topBar->addWidget(getFileButton);
    topBar->addWidget(sendFileButton);

    mainLayout->addWidget(pathWidget);
    mainLayout->addWidget(buttonWidget);
    //mainLayout->addLayout(topBar);

    // =====================================================
    // TOTAL BAR
    // =====================================================

    totalBar = new QProgressBar;

    totalBar->setRange(0,100);
    totalBar->setValue(0);
    totalBar->setFormat("GENEL İLERLEME : %p%");
    totalBar->setFixedHeight(h*0.4);

    mainLayout->addWidget(totalBar);

    // =====================================================
    // SCROLL
    // =====================================================

    scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setSizePolicy(QSizePolicy::Expanding,
                          QSizePolicy::Ignored);
    //scroll->setMinimumHeight(0);
    container = new QWidget;
    //container->adjustSize();
    listLayout = new QVBoxLayout(container);

    listLayout->setSpacing(2);
    listLayout->setContentsMargins(0,0,0,0);

    listLayout->addStretch();

    scroll->setWidget(container);

    mainLayout->addWidget(scroll);
}


void ProgressDialog::onDiscoveryButtonClicked()
{
    emit sendDiscovery();
}

void ProgressDialog::onGetFileButtonClicked()
{
    bool ok = false;
    QString myfile = QInputDialog::getText(
        nullptr,
        "Dosya Adı",
        "Masaüstünden Getirilecek Dosyayı Yazınız\n"
        "abc.txt\n"
        "deneme\n"
        "*.txt vb.",
        QLineEdit::Normal,
        "",
        &ok
    );

    if(!ok || myfile.isEmpty())
    {
        qDebug() << "Kullanıcı iptal etti";
        return;
    }

    SourceType mySourceType;

    QString text = myfile.trimmed();

    if(text.contains('*') || text.contains('?'))
    {
        mySourceType = SourceType::Pattern;
    }
    else if(text.endsWith('/') || text.endsWith('\\'))
    {
        mySourceType = SourceType::Directory;
    }
    else
    {
        mySourceType = SourceType::File;
    }

   qDebug() << "istenilen dosya:" << myfile;
   emit getFile(myfile,mySourceType);

}

void ProgressDialog::selectAllButtonClicked()
{
    for(ClientRow *client : clients)
    {
        client->select->setChecked(true);
    }

    emit updateSelectClients();
}

void ProgressDialog::unSelectAllButtonClicked()
{
    for(ClientRow *client : clients)
    {
        client->select->setChecked(false);
    }

    emit updateSelectClients();
}

void ProgressDialog::updateProgress(const QString &ip,
                                    qint64 received,qint64 total,int percent,
                                    QString clientHostName)
{
    if(!clients.contains(ip))
    {
        ClientRow *client = new ClientRow;

        // =====================================================
        // ROW
        // =====================================================

        QFrame *row = new QFrame;

        row->setSizePolicy(QSizePolicy::Expanding,
                           QSizePolicy::Fixed);

        row->setFixedHeight(h*0.45);
        row->setStyleSheet(QString(R"(
        QFrame {
            background-color: #ffffff;
            border: 1px solid #e6ebf2;
            border-radius: 8px;
            font-size:%1px;
        }
        QFrame:hover {
            border: 1px solid #cfe2ff;
        }
        )").arg(font));

        auto *shadow = new QGraphicsDropShadowEffect;

        shadow->setBlurRadius(6);
        shadow->setOffset(0,1);
        shadow->setColor(QColor(0,0,0,15));

        row->setGraphicsEffect(shadow);

        // =====================================================
        // LAYOUT
        // =====================================================

        QHBoxLayout *layout = new QHBoxLayout(row);
        layout->setContentsMargins(8,2,8,2);
        layout->setSpacing(6);

        // =====================================================
        // IP
        // =====================================================

        client->ip = new QLabel(ip);

        client->ip->setFixedWidth(w);

        client->ip->setStyleSheet(
            "color:#1f2a37;"
            "font-weight:600;"
        );

        // =====================================================
        // HOSTNAME
        // =====================================================

        client->hostName = new QLabel(clientHostName);

        client->hostName->setFixedWidth(h*1.4);

        client->hostName->setStyleSheet(
            "color:#6b7a90;"
        );

        // =====================================================
        // PROGRESS
        // =====================================================

        client->progresBar = new QProgressBar;

        client->progresBar->setRange(0,100);
        client->progresBar->setValue(0);
        client->progresBar->setFormat("%p%");

        client->progresBar->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Fixed
        );

        client->progresBar->setStyleSheet(QString(R"(
        QProgressBar {
            border: none;
            border-radius: 4px;
            background-color: #edf2f7;
            text-align: center;
            color: 1f2a37;
            font-size:%1px;

        }

        QProgressBar::chunk {
            background-color: #2f80ed;
            border-radius: 4px;
        }
        )").arg(font));

        // =====================================================
        // SPEED
        // =====================================================

        client->speedLabel = new QLabel("0 MB/s");

        client->speedLabel->setAlignment(Qt::AlignCenter);

        client->speedLabel->setFixedWidth(h*1.4);

        client->speedLabel->setStyleSheet(
            "color:#4b5563;"
        );

        // =====================================================
        // TIME
        // =====================================================

        client->timeLabel = new QLabel("00:00");

        client->timeLabel->setAlignment(Qt::AlignCenter);

        client->timeLabel->setFixedWidth(h*0.9);

        client->timeLabel->setStyleSheet(
            "color:#6b7280;"
        );

        // =====================================================
        // CHECKBOX
        // =====================================================

        client->select = new QCheckBox;

        client->select->setChecked(true);

        client->select->setStyleSheet(R"(

        QCheckBox {
            spacing: 0px;
        }

        QCheckBox::indicator {
            width: 14px;
            height: 14px;
            border-radius: 4px;
            border: 1px solid #cfd8e3;
            background-color: white;
        }

        QCheckBox::indicator:checked {
            background-color: #2f80ed;
            border: 1px solid #2f80ed;
        }

        )");

        connect(client->select,
                &QCheckBox::clicked,
                this,
                [=](bool checked)
        {
            clients[ip]->select->setChecked(checked);

            emit updateSelectClients();
        });

        // =====================================================
        // ADD
        // =====================================================

        layout->addWidget(client->ip);
        layout->addWidget(client->hostName);
        layout->addWidget(client->progresBar,1);
        layout->addWidget(client->speedLabel);
        layout->addWidget(client->timeLabel);
        layout->addWidget(client->select);
        listLayout->insertWidget(listLayout->count()-1,row);
        clients[ip] = client;
    }

    clients[ip]->progresBar->setValue(percent);
    progressClientEdit(ip,"-1");

    qint64 now = QDateTime::currentMSecsSinceEpoch();

    clients[ip]->diffTime =
            now - clients[ip]->lastTime;

    clients[ip]->lastTime = now;

    /**************************/
    if(clients[ip]->startTime == 0)
    {
        clients[ip]->startTime = now;
    }

    qint64 elapsed =
            now - clients[ip]->startTime;

    double sec = elapsed / 1000.0;

    if(sec < 0.1)
        sec = 0.1;
    /***************************************/
    //double transferredMb = received;
    /*double transferredMb =
            ((double)clients[ip]->totalFileSize *
             percent / 100.0)
            / (1024.0*1024.0);*/
    double speed =
            (received/ (1024.0*1024.0)) / sec;

    /********************************/
    clients[ip]->speedLabel->setText(
        QString("%1 MB/s")
        .arg(speed,0,'f',1));


    /************************************/
    int totalSec = sec;

    int min = totalSec / 60;

    int remainSec = totalSec % 60;

    clients[ip]->timeLabel->setText(
        QString("%1:%2")
        .arg(min,2,10,QChar('0'))
        .arg(remainSec,2,10,QChar('0')));
    /*******************************/

    updateTotalProgressBars();
    adjustSize();
    resize(w*6,
           pathWidget->height()
           +buttonWidget->height()
           + totalBar->height()
           + container->height()+h);
}

void ProgressDialog::allProgressStart(qint64 totalSize)
{
    //qDebug()<<"allProgressStart";
    for(ClientRow *client : clients)
    {
        progressClientEdit(client->ip->text(),"-1");
        client->progresBar->setValue(0);
        client->totalFileSize=totalSize;
        client->startTime=QDateTime::currentMSecsSinceEpoch();
        client->progresBar->setFormat("READY");

    }
        updateTotalProgressBars();
}

void ProgressDialog::updateTotalProgressBars()
{
    int sum = 0;
    int selectCount = 0;
    qint64 averageTime = 0;
    qint64 sumTime = 0;
    qint64 fileSize;
    for(ClientRow *client : clients)
    {
        if(client->select->isChecked())
        {
            sum += client->progresBar->value();
            sumTime += client->diffTime;
            selectCount++;
            fileSize=client->totalFileSize;
        }
    }

    totalBar->setValue(
        selectCount == 0
        ? 0
        : sum / selectCount
    );

    if(totalBar->value() > 95)
    {
        updateTotalBarText("Dosya/Dizin Gönderme Tamamlandı");
        /*averageTime = sumTime / selectCount;
        qDebug()<<"denemem"<<averageTime<<fileSize;
        if((fileSize<10000)||(averageTime>2000))
            emit sendFileTransferAllDone(500);
        else
            emit sendFileTransferAllDone(averageTime);*/
    }
}

void ProgressDialog::markDone(const QString &ip)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        clients[ip]->progresBar->setFormat("DONE");
    updateTotalProgressBars();
    }
}

void ProgressDialog::markDebInstallStart(const QString &ip)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        clients[ip]->progresBar->setFormat("DEB INSTALL START");
    }
}

void ProgressDialog::markDebInstallDone(const QString &ip,
                                        const QString &status)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        if(status=="0"){
        clients[ip]->progresBar->setFormat("DEB INSTALL SUCCESSFULLY DONE");
        progressClientEdit(ip,"0");
        }else{
        clients[ip]->progresBar->setFormat("DEB INSTALL FAILED");
        progressClientEdit(ip,"1");
        }
    }
}

void ProgressDialog::markScriptInstallStart(const QString &ip)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        clients[ip]->progresBar->setFormat("SCRIPT EXEC START");
    }
}

void ProgressDialog::markScriptInstallDone(const QString &ip,
                                           const QString &status)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        if(status=="0"){
        clients[ip]->progresBar->setFormat("SCRIPT EXECUTE SUCCESSFULLY DONE");
        progressClientEdit(ip,"0");
        }else
        {
        clients[ip]->progresBar->setFormat("SCRIPT EXECUTE FAILED");
        progressClientEdit(ip,"1");
        }
    }
}
void ProgressDialog::progressClientEdit(const QString &ip,
                                           const QString &status)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        if(status=="-1"){
        //clients[ip]->progresBar->setFormat("SCRIPT EXECUTE SUCCESSFULLY DONE");
            clients[ip]->progresBar->setStyleSheet(QString(R"(
            QProgressBar {
                border: none;
                border-radius: 4px;
                background-color: #edf2f7;
                text-align: center;
                color: 1f2a37;
                font-size:%1px;

            }

            QProgressBar::chunk {
                background-color: #2f80ed;
                border-radius: 4px;
            }
            )").arg(font));
        }
        else if(status=="0"){
        //clients[ip]->progresBar->setFormat("SCRIPT EXECUTE SUCCESSFULLY DONE");
            clients[ip]->progresBar->setStyleSheet(QString(R"(
            QProgressBar {
                border: none;
                border-radius: 4px;
                background-color: #edf2f7;
                text-align: center;
                color: 1f2a37;
                font-size:%1px;

            }

            QProgressBar::chunk {
                background-color: #77ee77;
                border-radius: 4px;
            }
            )").arg(font));
        }
        else if(status=="1"){
            //clients[ip]->progresBar->setFormat("SCRIPT EXECUTE FAILED");
            clients[ip]->progresBar->setStyleSheet(QString(R"(
            QProgressBar {
                border: none;
                border-radius: 4px;
                background-color: #edf2f7;
                text-align: center;
                color: 1f2a37;
                font-size:%1px;

            }

            QProgressBar::chunk {
                background-color: #ee7777;
                border-radius: 4px;
            }
            )").arg(font));
        }
    }
}


void ProgressDialog::updateTotalBarText(const QString &text)
{
    totalBar->setFormat(text);
}

QMenu *ProgressDialog::sendMenu()
{   QMenu *menu = new QMenu(this);
    //QString font="12";
   QPushButton *desktopButton= new QPushButton;
   desktopButton->setFixedSize(w*1.5, h*0.7);
   desktopButton->setIcon(QIcon(":/icons/senddesktop.svg"));
   desktopButton->setIconSize(QSize(w, h*0.6));
   desktopButton->setText(tr(" Masaüstüne"));
   desktopButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   desktopButton->setFlat(true);

   connect(desktopButton, &QPushButton::clicked, [=]() {

       emit startTransfer("/tmp",TransferLocation::FileCopyDesktop);
       menu->close();
});

   QPushButton *homeButton= new QPushButton;
   homeButton->setFixedSize(w*1.5, h*0.7);
   homeButton->setIcon(QIcon(":/icons/sendhome.svg"));
   homeButton->setIconSize(QSize(w, h*0.6));
   homeButton->setText(tr("Ev Dizini"));
   homeButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   homeButton->setFlat(true);

   connect(homeButton, &QPushButton::clicked, [=]() {

       emit startTransfer("/tmp",TransferLocation::FileCopyHome);
       menu->close();
});

   QPushButton *tmpButton= new QPushButton;
   tmpButton->setFixedSize(w*1.5, h*0.7);
   tmpButton->setIcon(QIcon(":/icons/sendtmp.svg"));
   tmpButton->setIconSize(QSize(w, h*0.6));
   tmpButton->setText(tr(" tmp"));
   tmpButton->setStyleSheet("Text-align:left;  font-size:"+font+"px;");
   tmpButton->setFlat(true);

   connect(tmpButton, &QPushButton::clicked, [=]() {

       emit startTransfer("/tmp",TransferLocation::FileCopyTmp);
       menu->close();
});

   QPushButton *askButton= new QPushButton;
   askButton->setFixedSize(w*1.5, h*0.7);
   askButton->setIcon(QIcon(":/icons/sendcustom.svg"));
   askButton->setIconSize(QSize(w, h*0.6));
   askButton->setText(tr(" Özel Konum"));
   askButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   askButton->setFlat(true);

   connect(askButton, &QPushButton::clicked, [=]() {
       bool ok = false;
       QString dir = QInputDialog::getText(
           nullptr,
           "Hedef Konum",
           "Dosya nereye kopyalansın?\n"
           "Örnek:\n"
           "/home/user/Desktop\n"
           "/tmp\n"
           "/opt/data",
           QLineEdit::Normal,
           QDir::homePath(),
           &ok
       );

       if(!ok || dir.isEmpty())
       {
           qDebug() << "Kullanıcı iptal etti";
           return -1;
       }

       // (opsiyonel) basit doğrulama
       if(!dir.startsWith("/"))
       {
           qDebug() << "Geçersiz path";
           return -1;
       }
       //qDebug() << "Custom target:" << dir;


      emit startTransfer(dir,TransferLocation::FileCopyCustom);
       menu->close();
});

   QPushButton *scriptExecButton= new QPushButton;
   scriptExecButton->setFixedSize(w*1.5, h*0.7);
   scriptExecButton->setIcon(QIcon(":/icons/scriptexec.svg"));
   scriptExecButton->setIconSize(QSize(w, h*0.6));
   scriptExecButton->setText(tr(" Script Çalıştır"));
   scriptExecButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   scriptExecButton->setFlat(true);

   connect(scriptExecButton, &QPushButton::clicked, [=]() {

       emit startTransfer("/tmp",TransferLocation::ScriptExecute);
       menu->close();
});
   QPushButton *debInstallButton= new QPushButton;
   debInstallButton->setFixedSize(w*1.5, h*0.7);
   debInstallButton->setIcon(QIcon(":/icons/debinstall.svg"));
   debInstallButton->setIconSize(QSize(w, h*0.6));
   debInstallButton->setText(tr(" Paket Kur"));
   debInstallButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   debInstallButton->setFlat(true);

   connect(debInstallButton, &QPushButton::clicked, [=]() {

      emit startTransfer("/tmp",TransferLocation::DebInstall);
       menu->close();
});

    /******************************************************/

    auto widget = new QWidget;
    auto layout = new QGridLayout(widget);
    layout->setContentsMargins(0, 0, 0,5);
    layout->setVerticalSpacing(0);
  //  layout->setColumnMinimumWidth(0, 37);
    layout->addWidget(desktopButton, 5,0,1,2);
    layout->addWidget(homeButton, 15,0,1,2);
    layout->addWidget(tmpButton, 20,0,1,2);
    layout->addWidget(askButton, 25,0,1,2);
    layout->addWidget(scriptExecButton, 30,0,1,2);
    layout->addWidget(debInstallButton, 35,0,1,2);

     // add a widget action to the context menu
    auto wa = new QWidgetAction(this);
  //  wa->setIcon(QIcon(":/icon1"));
    wa->setDefaultWidget(widget);
    menu->addAction(wa);

     menu->setStyleSheet("QMenu { width: 130 px; height: 210 px; font-size:"+font+"px;}");
   return menu;
}
