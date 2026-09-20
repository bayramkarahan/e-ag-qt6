#include "monitorclient.h".h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include<QApplication>
#include<QScreen>
#include <QGuiApplication>
#include<udpservice.h>
#include<update.h>
#include<pathwidget.h>
#include<buttonwidget.h>
#include<listeWidget.h>

MonitorClient::MonitorClient(QWidget *parent)
    : QDialog(parent)
{
    mainrolinfo = getRole();
    infoLabel = new QLabel;
    infoLabel->setText(
        QString("Rol: %1 -- Ip: %2")
            .arg(mainrolinfo.role == "sender" ? "Gönderici" : "Alıcı")
            .arg(getLocalIp())
    );
    updConnect();
//QCheckBox("SToReceiver").checkState()

    /************************version*******************************************/

    QProcess process;
    process.start("/bin/bash", {"-c", "dpkg -s dds | grep -i '^Version:' | awk '{print $2}'"});
    process.waitForFinished();
    QString version = QString::fromUtf8(process.readAll()).trimmed();
    setWindowTitle("DDS (Dosya Dağıtım Sistemi) "+version);

    //QRect screenGeometry = QApplication::desktop()->screenGeometry();
    //QSize screenSize = qApp->screens()[0]->size();
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    h=screenGeometry.height()/15.1;
    w=h*2;
    int nfont=h/4;
    //qDebug()<<nfont;
    font=QString::number(nfont);

    //qDebug()<<w<<h<<screenSize.height()/16;
    // =====================================================
    // STYLE
    // =====================================================

    ////setStyleSheet(QString(R"()").arg(font));
    QFile file(":/style/style.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString style = QString::fromUtf8(file.readAll());

        style.replace("%FONT_SIZE%", QString::number(nfont));

        setStyleSheet(style);
    }
    /**************************************************/
    /**************************************************/
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6,6,6,6);
    mainLayout->setSpacing(4);

    // =====================================================
    // TOTAL BAR
    // =====================================================

    totalBar = new QProgressBar;

    totalBar->setRange(0,100);
    totalBar->setValue(0);
    totalBar->setFormat("%p%");
    totalBar->setFixedHeight(h*0.4);


    // =====================================================
    // SCROLL
    // =====================================================
    scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setSizePolicy(QSizePolicy::Expanding,
                          QSizePolicy::Preferred);
    scroll->setMaximumHeight(h*10);
    //scroll->setMinimumHeight(0);
    scroll->setFixedHeight(0);
    scroll->setObjectName("scroll");

    container = new QWidget;
    container->setFixedHeight(0);

    listLayout = new QVBoxLayout(container);

    listLayout->setSpacing(2);
    listLayout->setContentsMargins(0,0,0,0);

    listLayout->addStretch();

    scroll->setWidget(container);



    pathWidget=createPathWidget();
    buttonWidget=createButtonWidget();
    header=createHeader();
    mainLayout->addWidget(pathWidget);
    mainLayout->addWidget(buttonWidget);
    mainLayout->addWidget(totalBar);
    mainLayout->addWidget(header);
    mainLayout->addWidget(scroll);

    int by= pathWidget->height()+buttonWidget->height()+ totalBar->height();+ scroll->height();
    resize(w*9,by);
    // listeyi hazırla
    onDiscoveryButtonClicked();
    // =====================================================
    // BUTTON VISIBLE
    // =====================================================

    if(mainrolinfo.sendToSender||mainrolinfo.sendToReceiver)
        sendFileButton->setVisible(true);
    else
        sendFileButton->setVisible(false);

    if(mainrolinfo.receiver)
        getFileButton->setVisible(true);
    else
        getFileButton->setVisible(false);


    /********************************/
    // move
    int x = (screenGeometry.width()/2-(this->width()/2));
    int y = (h*2);
    this->move(x, y);
}


void MonitorClient::onDiscoveryButtonClicked()
{

    while (QLayoutItem *item = listLayout->takeAt(0))
    {
        if (item->widget())item->widget()->deleteLater();
        delete item;
    }
    clients.clear();
    sendMonitorDiscovery();

   mainrolinfo = getRole();
    /* if(mainrolinfo.sendToSender||mainrolinfo.sendToReceiver)
        sendFileButton->setVisible(true);
    else
       sendFileButton->setVisible(false);

    if(mainrolinfo.receiver)
        getFileButton->setVisible(true);
    else
        getFileButton->setVisible(false);
    onDiscoveryButtonClicked();*/



    /*for(ClientRow *cl : clients)
    {
        //client->select->setChecked(true);
        updateProgress(cl->ip->text(),0,0,0,cl->hostName->text()
        ,cl->role->text(),cl->sendToSender,cl->sendToReceiver,cl->receiver,cl->getToOther);
    }*/

  /* QTimer::singleShot(500, [this]()
        {
       */

 /*  this->updateProgress(90,"192.168.1.190",0,0,20,"190","sender",true,true,true,false);
           this->updateProgress(3,"192.168.1.191",0,0,  30,"191","receiver",true,true,true,true);
           this->updateProgress(5,"192.168.1.192",0,0, 60,"192","sender",true,true,true,true );
           this->updateProgress(7,"192.168.1.193",0,0, 70,"193","receiver",true,true,true ,true);
           this->updateProgress(9,"192.168.1.194", 0,0, 80,"194","sender",true,true,true,true);
           this->updateProgress(11,"192.168.1.195",0,0,20,"195","receiver",true,true,true ,true);
           this->updateProgress(68,"192.168.1.196",0,0, 30,"196","sender",true,true,true ,true);
           this->updateProgress(97,"192.168.1.197",0,0, 60,"197","receiver",true,true,true ,true);
           this->updateProgress(98,"192.168.1.198",0,0, 70,"198","sender",true,true,true ,true);
           this->updateProgress(99,"192.168.1.199", 0,0,80,"199","sender",true,true,true ,true);
           this->updateProgress(95,"192.168.2.195",0,0,20,"295","sender",true,true,true ,true);
*/
    //emit sendDiscovery();
    //});*/
}

void MonitorClient::getFileButtonClicked(SourceLocation sourceLocation)
{
    QInputDialog dlg(this);

    dlg.setWindowTitle("Dosya Adı");
    dlg.setLabelText(
        "Masaüstünden Getirilecek <b>Dosya</b> veya <b>Dizini</b> Yazınız<br>"
        "abc.txt<br>"
        "deneme<br>"
        "*.txt vb."
        );

    dlg.setStyleSheet(QString(R"(
    QInputDialog {
        background-color: #f5f7fb;
        color: #1f2a37;
        font-family: Arial;
        font-size: %1px;
    }

    /* BUTTONS */

    QPushButton {
        background-color: #ffffff;
        border: 1px solid #e5eaf2;
        padding: 5px 10px;
        border-radius: 8px;
        color: #1f2a37;
        /*font-weight:600;*/
        font-size: %1px;
    }

    QPushButton:hover {
        background-color: #f0f6ff;
        border: 1px solid #cfe2ff;
    }

    QPushButton:pressed {
        background-color: #e6f0ff;
    }

    /* PRIMARY BUTTON */

    QPushButton#saveButton {
        background-color: #256fd1;
        color: white;
        border: none;
    }

    QPushButton#saveButton:hover {
        background-color: #256fd1;
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
    )").arg(12));


    dlg.setTextValue("");
    dlg.resize(w*3, h*5);   // boyut
    QString myfile ;
    if (dlg.exec() == QDialog::Accepted&&(!dlg.textValue().isEmpty()))
    {
        myfile = dlg.textValue();
    }else
    {
        qDebug() << "Kullanıcı iptal etti";
        return;
    }

    SourceType mySourceType;
    mySourceType=fileTypeDetect(myfile);

   qDebug() << "istenilen dosya:" << myfile;
   sendReceiverData(myfile,mySourceType,sourceLocation,TargetLocation::FileCopyDesktop);


}

void MonitorClient::selectAllButtonClicked()
{
    for(ClientRow *client : clients)
    {
        client->select->setChecked(true);
    }

    emit updateSelectClients();
}

void MonitorClient::unSelectAllButtonClicked()
{
    for(ClientRow *client : clients)
    {
        client->select->setChecked(false);
    }

    emit updateSelectClients();
}

void MonitorClient::allProgressStart(qint64 totalSize)
{
    //qDebug()<<"allProgressStart";
    updateTotalBarText("%p%");
    for(ClientRow *client : clients)
    {
        progressClientEdit(client->ip->text(),"-1");
        client->progresBar->setValue(0);
        client->totalFileSize=totalSize;
        client->startTime=QDateTime::currentMSecsSinceEpoch();
        client->progresBar->setFormat("%p%");
        ///qDebug()<<"allProgressStart"<< client->progresBar->value();
        updateTotalProgressBars();
    }

}

void MonitorClient::updateTotalProgressBars()
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
    //qDebug()<<"totalBar"<<selectCount<<totalBar->value()<<sum;
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

void MonitorClient::markDone(const QString &ip)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        clients[ip]->progresBar->setFormat("DONE");
        //qDebug() << "markDone:" <<clients[ip]->progresBar->value();
        updateTotalProgressBars();
    }
}

void MonitorClient::markDebInstallStart(const QString &ip)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        clients[ip]->progresBar->setFormat("DEB INSTALL START");
    }
}

void MonitorClient::markDebInstallDone(const QString &ip,
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

void MonitorClient::markScriptInstallStart(const QString &ip)
{
    if(clients.contains(ip))
    {
        clients[ip]->progresBar->setValue(100);
        clients[ip]->progresBar->setFormat("SCRIPT EXEC START");
    }
}

void MonitorClient::markScriptInstallDone(const QString &ip,
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
void MonitorClient::progressClientEdit(const QString &ip,
                                           const QString &status)
{
    if(clients.contains(ip))
    {
        if(status=="-1"){
        //clients[ip]->progresBar->setFormat("SCRIPT EXECUTE SUCCESSFULLY DONE");
            clients[ip]->progresBar->setStyleSheet(QString(R"(
            QProgressBar {
                border: none;
                border-radius: 4px;
                background-color: transparent;
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
                background-color: transparent;
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
                background-color: transparent;
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


void MonitorClient::updateTotalBarText(const QString &text)
{
    totalBar->setFormat(text);
}

QMenu *MonitorClient::sendMenu()
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

       sendSenderData("/tmp",TargetLocation::FileCopyDesktop);
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

       sendSenderData("/tmp",TargetLocation::FileCopyHome);
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

       sendSenderData("/tmp",TargetLocation::FileCopyTmp);
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


       sendSenderData(dir,TargetLocation::FileCopyCustom);
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

       sendSenderData("/tmp",TargetLocation::ScriptExecute);
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

       sendSenderData("/tmp",TargetLocation::DebInstall);
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
    int wn=(w*1.2);
    QString w1=QString::number(wn);
    int hn=(h*4.3);
    QString h1=QString::number(hn);

     menu->setStyleSheet(QString("QMenu { width: %2 px; height: %3 px; font-size:%1px;}").arg(font).arg(w1).arg(h1));
   return menu;
}



QMenu *MonitorClient::getMenu()
{   QMenu *menu = new QMenu(this);
    //QString font="12";
   QPushButton *desktopButton= new QPushButton;
   desktopButton->setFixedSize(w*1.5, h*0.7);
   desktopButton->setIcon(QIcon(":/icons/senddesktop.svg"));
   desktopButton->setIconSize(QSize(w, h*0.6));
   desktopButton->setText(tr(" Masaüstü'nden"));
   desktopButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   desktopButton->setFlat(true);

   connect(desktopButton, &QPushButton::clicked, [=]() {

       getFileButtonClicked(SourceLocation::Desktop);
       menu->close();
});

   QPushButton *homeButton= new QPushButton;
   homeButton->setFixedSize(w*1.5, h*0.7);
   homeButton->setIcon(QIcon(":/icons/sendhome.svg"));
   homeButton->setIconSize(QSize(w, h*0.6));
   homeButton->setText(tr("Ev Dizini'nden"));
   homeButton->setStyleSheet("Text-align:left; font-size:"+font+"px;");
   homeButton->setFlat(true);

   connect(homeButton, &QPushButton::clicked, [=]() {

       getFileButtonClicked(SourceLocation::Home);
       menu->close();
});

   QPushButton *tmpButton= new QPushButton;
   tmpButton->setFixedSize(w*1.5, h*0.7);
   tmpButton->setIcon(QIcon(":/icons/sendtmp.svg"));
   tmpButton->setIconSize(QSize(w, h*0.6));
   tmpButton->setText(tr(" tmp'den"));
   tmpButton->setStyleSheet("Text-align:left;  font-size:"+font+"px;");
   tmpButton->setFlat(true);

   connect(tmpButton, &QPushButton::clicked, [=]() {

       getFileButtonClicked(SourceLocation::Tmp);
       menu->close();
});

   QPushButton *askButton= new QPushButton;
   askButton->setFixedSize(w*1.5, h*0.7);
   askButton->setIcon(QIcon(":/icons/sendcustom.svg"));
   askButton->setIconSize(QSize(w, h*0.6));
   askButton->setText(tr(" Özel Konum'dan"));
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


       //getFileButtonClicked(dir,SourceLocation::Custom);
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
    //layout->addWidget(askButton, 25,0,1,2);

     // add a widget action to the context menu
    auto wa = new QWidgetAction(this);
  //  wa->setIcon(QIcon(":/icon1"));
    wa->setDefaultWidget(widget);
    menu->addAction(wa);
    int wn=(w*1.5);
    QString w1=QString::number(wn);
    int hn=(h*2.2);
    QString h1=QString::number(hn);

     menu->setStyleSheet(QString("QMenu { width: %2 px; height: %3 px; font-size:%1px;}").arg(font).arg(w1).arg(h1));
   return menu;
}


RoleInfo MonitorClient::getRole()
{
    localDir="/usr/share/dds/";
    DatabaseHelper *db=new DatabaseHelper(localDir+"dds.json");
    QJsonArray dizi=db->Oku();
     RoleInfo info;
    if(dizi.count()>0)
    {
        QJsonValue item=dizi.first();
        QJsonObject veri=item.toObject();

        info.role=veri["role"].toString();
        info.sendToSender=veri["sendToSender"].toBool();
        info.sendToReceiver=veri["sendToReceiver"].toBool();
        info.receiver=veri["receiver"].toBool();
        info.getToOther=veri["getToOther"].toBool();
        info.root=veri["root"].toBool();
        //qDebug()<<"info"<<info.role<<info.sender<<info.receiver;

        return info;
    }
    return info;
}

void MonitorClient::sortClients()
{
    QList<ClientRow*> rows = clients.values();

    std::sort(rows.begin(),
              rows.end(),
              [](ClientRow *a, ClientRow *b)
              {
                  return a->hostName->text().toLower()
                  < b->hostName->text().toLower();
              });

    for(int i = 0; i < rows.size(); ++i)
    {
        listLayout->removeWidget(rows[i]->row);
        listLayout->insertWidget(i, rows[i]->row);
    }
}

bool MonitorClient::isUserSpecialDirectory(const QString &path)
{
    QString targetPath = QDir(path).canonicalPath();

    QFile file(QDir::homePath() + "/.config/user-dirs.dirs");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    while(!file.atEnd())
    {
        QString line = QString::fromUtf8(file.readLine()).trimmed();

        if(!line.startsWith("XDG_") || !line.contains("$HOME"))
            continue;

        int firstQuote = line.indexOf('"');
        int lastQuote  = line.lastIndexOf('"');

        if(firstQuote == -1 || lastQuote <= firstQuote)
            continue;

        QString xdgPath =
                line.mid(firstQuote + 1,
                         lastQuote - firstQuote - 1);

        xdgPath.replace("$HOME", QDir::homePath());

        if(QDir(xdgPath).canonicalPath() == targetPath)
            return true;
    }

    return false;
}

QString MonitorClient::getDesktopPathFromHome(const QString &home)
{
    QFile file(home + "/.config/user-dirs.dirs");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd())
        {
            QString line = file.readLine();

            if(line.startsWith("XDG_DESKTOP_DIR"))
            {
                int firstQuote = line.indexOf('"');
                int lastQuote = line.lastIndexOf('"');

                if(firstQuote != -1 && lastQuote != -1 && lastQuote > firstQuote)
                {
                    QString path = line.mid(firstQuote + 1, lastQuote - firstQuote - 1);
                    path.replace("$HOME", home);
                    return path;
                }
            }
        }
    }

    // fallback
    return home + "/Desktop";
}


void MonitorClient::about()
{
    QString yil = QString::number(QDate::currentDate().year());
    /************************version*******************************************/

    QProcess process;
    process.start("/bin/bash", {"-c", "dpkg -s dds | grep -i '^Version:' | awk '{print $2}'"});
    process.waitForFinished();

    QString version = QString::fromUtf8(process.readAll()).trimmed();


    QString aboutText = QStringLiteral(R"(
<b>DDS</b> - Dosya Dağıtım Sistemi<br>
Sürüm: %2<br><br>

Bu uygulama Linux tabanlı sistemlerde (<b>Pardus</b>) çalışır;<br>
Bulunduğu ağda dosya dağıtma ve toplamak için geliştirilmiştir.<br><br>
*****************************************************************************<br>
Built with Qt (LGPLv3)<br>
© The Qt Company Ltd<br>
*****************************************************************************<br>

Copyright (C) %1 Bayram KARAHAN<br>
github.com/bayramkarahan/dds<br>
bayramkarahan.com.tr<br>
bayramkarahan.blogspot.com<br>
bayramk@gmail.com<br>
*****************************************************************************<br>
This program is free software: you can redistribute it and/or modify it under the terms of the<br>
GNU General Public License as published by the Free Software Foundation, either version 3 of the License,<br>
or (at your option) any later version.<br><br>
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;<br>
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br>
See the GNU General Public License for more details.<br><br>
You should have received a copy of the GNU General Public License along with this program;<br>
if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
)").arg(yil).arg(version);


    QMessageBox::about(this, tr("DDS Hakkında"), aboutText);
}
