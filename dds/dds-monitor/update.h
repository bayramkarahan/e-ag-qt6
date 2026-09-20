#ifndef UPDATE_H
#define UPDATE_H
void MonitorClient::updateProgress(quint64 transferId,const QString &ip,
                                    qint64 received, qint64 total, int percent,
                                    QString clientHostName, QString role,bool rolesendToSender,
                                    bool rolesendToReceiver,bool rolereceiver,bool rolegetToOther)
{
    //qDebug()<<"updateProgress"<<transferId;
    // hiçbir rol seçili değil
    if (!mainrolinfo.sendToSender && !mainrolinfo.sendToReceiver)
        return;

    // sender filtrele
    if (role == "sender" && !mainrolinfo.sendToSender)
        return;

    // receiver filtrele
    if (role == "receiver" && !mainrolinfo.sendToReceiver)
        return;

    // dosya gönderilemez
    if(!rolegetToOther) return;

    // Dosya Kabul Edilemez
    if(!rolereceiver)
        return;

    // buraya geldiyse listelenebilir

    if(!clients.contains(ip))
    {
        ClientRow *client = new ClientRow;
        client->transferId=transferId;

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
           /* border-radius: 8px;*/
            font-size:%1px;
        }
        QFrame:hover {
            /*border: 2px solid #1f2a37;*/
            background-color:#cfe2ff;
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
            "background-color: transparent;"
                   " border: none;"
            );

        // =====================================================
        // HOSTNAME
        // =====================================================

        client->hostName = new QLabel(clientHostName);

        client->hostName->setFixedWidth(h*1.4);

        client->hostName->setStyleSheet(
            "color:#6b7a90;"
                    "background-color: transparent;"
                    " border: none;"
            );
        // =====================================================
        // ROLE
        // =====================================================
        client->role = new QLabel(role);// == "sender" ? "Gönderici" : "Alıcı");
        client->role->setFixedWidth(w*0.8);
        client->role->setStyleSheet(
            "color:#ff0000;"
                    "background-color: transparent;"
                    " border: none;"
            );
        QLabel *tempRoleLabel = new QLabel(role== "sender" ? "Gönderici" : "Alıcı");
        tempRoleLabel->setFixedWidth(w*0.8);
        tempRoleLabel->setStyleSheet(
            "color:#ff0000;""background-color: transparent;"" border: none;");
        // =====================================================
        // ROLESENDtoSender
        // =====================================================
        client->sendToSender = new QCheckBox("");
        //client->sendToSender->setFixedWidth(w*0.9);
        client->sendToSender->setChecked(rolesendToSender);
        connect(client->sendToSender,&QCheckBox::clicked,this,[=](bool checked){
                    clients[ip]->sendToSender->setChecked(checked);
                });
        QWidget *sendToSender = new QWidget;
        sendToSender->setFixedWidth(w*0.8);
        QHBoxLayout *sendToSenderl = new QHBoxLayout(sendToSender);
        sendToSenderl->setContentsMargins(0,0,0,0);
        sendToSenderl->addWidget(client->sendToSender);
        sendToSenderl->setAlignment(Qt::AlignCenter);
        // =====================================================
        // ROLESENDtoReceiver
        // =====================================================
        client->sendToReceiver = new QCheckBox("");
        //client->sendToReceiver->setFixedWidth(w*0.7);
        client->sendToReceiver->setChecked(rolesendToReceiver);
         connect(client->sendToReceiver,&QCheckBox::clicked,this, [=](bool checked) {
                    clients[ip]->sendToReceiver->setChecked(checked);
                });
        QWidget *sendToReceiver = new QWidget;
        sendToReceiver->setFixedWidth(w/2);
        QHBoxLayout *sendToReceiverl = new QHBoxLayout(sendToReceiver);
        sendToReceiverl->setContentsMargins(0,0,0,0);
        sendToReceiverl->addWidget(client->sendToReceiver);
        sendToReceiverl->setAlignment(Qt::AlignCenter);
        // =====================================================
        // ROLERECEIVER
        // =====================================================
        client->receiver = new QCheckBox("");
        //client->receiver->setFixedWidth(w*0.6);
        client->receiver->setChecked(rolereceiver);
        connect(client->receiver,&QCheckBox::clicked,this, [=](bool checked){
                    clients[ip]->receiver->setChecked(checked);
                });
        QWidget *receiver = new QWidget;
        receiver->setFixedWidth(w/2);
        QHBoxLayout *receiverl = new QHBoxLayout(receiver);
        receiverl->setContentsMargins(0,0,0,0);
        receiverl->addWidget(client->receiver);
        receiverl->setAlignment(Qt::AlignCenter);
        // =====================================================
        // ROLEGETTOOTHER
        // =====================================================
        client->getToOther = new QCheckBox("");
        //client->getToOther->setFixedWidth(w*0.8);
        client->getToOther->setChecked(rolegetToOther);
        connect(client->getToOther,&QCheckBox::clicked,this,[=](bool checked){
            clients[ip]->getToOther->setChecked(checked);
        });

        QWidget *getToOther = new QWidget;
        getToOther->setFixedWidth(w*0.8);
        QHBoxLayout *getToOtherl = new QHBoxLayout(getToOther);
        getToOtherl->setContentsMargins(0,0,0,0);
        getToOtherl->addWidget(client->getToOther);
        getToOtherl->setAlignment(Qt::AlignCenter);


        // =====================================================
        // BUTTON SAVE
        // =====================================================
        QToolButton *settingButton= new QToolButton;
        settingButton->setFixedWidth(w);
        ///settingButton->setIconSize(QSize(w*0.4,h*0.4));
        settingButton->setStyleSheet("Text-align:center");
        ///settingButton->setIcon(QIcon(":/icons/settings.svg"));
        settingButton->setAutoRaise(true);
        //settingButton->setToolButtonStyle(Qt::);
        settingButton->setStyleSheet(
            "color:#6b7a90;"
            "padding: 0px 0px;"
            "border: 1px solid #e5eaf2;"
            "background-color: #f0f6ff;"
            );
        settingButton->setText("İzinleri Kaydet");
        connect(settingButton, &QPushButton::clicked, [=]() {
            ///qDebug()<<client->ip->text();
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("İzinleri Kaydet");
            msgBox.setText("İzinler değiştirmek istiyor musunuz?");
            msgBox.setIcon(QMessageBox::Question);
            QPushButton *saveButton =
                msgBox.addButton("Kaydet",
                                 QMessageBox::AcceptRole);

            QPushButton *cancelButton =
                msgBox.addButton("Vazgeç",
                                 QMessageBox::RejectRole);

            msgBox.exec();

            if(msgBox.clickedButton() == saveButton)
            {
                ///saveSettings();
                sendRoleStatus(client->ip->text());
            }
            else if(msgBox.clickedButton() == cancelButton)
            {
                return;
            }

        });
        // =====================================================
        // PROGRESS
        // =====================================================
        client->progresBar = new QProgressBar;
        client->progresBar->setObjectName("clientprogresBar");
        client->progresBar->setRange(0,100);
        client->progresBar->setValue(0);
        client->progresBar->setFormat("%p%");
        client->progresBar->setMinimumWidth(w*1.5);
        client->progresBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        ///client->progresBar->setStyleSheet("border: 1px solid #e5eaf2;");
        // =====================================================
        // SPEED
        // =====================================================

        client->speedLabel = new QLabel("0 MB/s");

        client->speedLabel->setAlignment(Qt::AlignCenter);

        client->speedLabel->setFixedWidth(h*1.4);

        client->speedLabel->setStyleSheet(
            "color:#4b5563;"
                    "background-color: transparent;"
                    " border: none;"
            );

        // =====================================================
        // TIME
        // =====================================================

        client->timeLabel = new QLabel("00:00");

        client->timeLabel->setAlignment(Qt::AlignCenter);

        client->timeLabel->setFixedWidth(h*0.9);

        client->timeLabel->setStyleSheet(
            "color:#6b7280;"
            "background-color: transparent;"
            " border: none;"
            );
        // =====================================================
        // STOPBUTTON
        // =====================================================
        QToolButton *stopButton= new QToolButton;
        stopButton->setIcon(QIcon(":/icons/stop.svg"));
        stopButton->setToolTip("Transferi Durdur");
        stopButton->setFixedWidth(w/5);
        stopButton->setStyleSheet("Text-align:center");
        stopButton->setAutoRaise(true);
        stopButton->setStyleSheet(R"(

        QToolButton {
            color: #6b7a90;
            padding: 0px;
            border: 1px solid transparent;
            border-radius: 4px;
            background-color: transparent;
        }

        QToolButton:hover {
            background-color: #256fd1;
            border: 1px solid #cfe2ff;
        }

        QToolButton:pressed {
            background-color: #d16f25;
            border: 1px solid #b7d4ff;
        }

        )");
        connect(stopButton,&QToolButton::clicked,this,[=](){
            sendStopTransfer(client->transferId,ip);
        });
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
        layout->addWidget(tempRoleLabel);

        if(getRole().root){
        layout->addWidget(sendToSender);
        layout->addWidget(sendToReceiver);
        layout->addWidget(receiver);
        layout->addWidget(getToOther);
        layout->addWidget(settingButton);
        }
        layout->addWidget(client->progresBar,1);
        layout->addWidget(client->speedLabel);
        layout->addWidget(client->timeLabel);
        layout->addWidget(stopButton);
        layout->addWidget(client->select);
        listLayout->insertWidget(listLayout->count()-1,row);
        clients[ip] = client;
        client->row = row;
        sortClients();
    }
    clients[ip]->progresBar->setValue(percent);
    clients[ip]->transferId=transferId;
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

    int contentHeight = clients.count() * (h*0.48);
    container->setFixedHeight(contentHeight);

    scroll->setFixedHeight(qMin(contentHeight+4, h*10));
    updateTotalProgressBars();
    adjustSize();
    int by=           pathWidget->height()
            +buttonWidget->height()
            + totalBar->height()
            + scroll->height();
   // qDebug()<<"by"<<by<<scroll->height();
    resize(w*8,by);
}

QFrame *MonitorClient::createHeader()
{
    ClientRow *client = new ClientRow;

    // =====================================================
    // ROW
    // =====================================================

    QFrame *row = new QFrame;

    row->setSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Fixed);

    row->setFixedHeight(h*1);
    row->setStyleSheet(QString(R"(
    QFrame {
        background-color: #dddddd;
        /*border: 1px solid #e6ebf2;*/
        border-radius: 0px;
        font-size:%1px;
    }
    QFrame:hover {
        /*border: 1px solid #cfe2ff;*/
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
    client->ip = new QLabel("Ip");
    client->ip->setFixedWidth(w);
    client->ip->setStyleSheet("color:#1f2a37;font-weight:800;");
    // =====================================================
    // HOSTNAME
    // =====================================================
    client->hostName = new QLabel("Bilgisayar");
    client->hostName->setFixedWidth(h*1.4);
    client->hostName->setStyleSheet("color:#1f2a37;font-weight:800;");
    // =====================================================
    // ROLE
    // =====================================================
    client->role = new QLabel("Rolü");
    client->role->setFixedWidth(w*0.8);
    client->role->setStyleSheet("color:#1f2a37;font-weight:800;");
    // =====================================================
    // ROLESENDtoSender
    // =====================================================

    QWidget *sendToSender = new QWidget;
    sendToSender->setFixedWidth(w*0.8);
    sendToSender->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //sendToSender->setStyleSheet("color:#1f2a37;border: 1px solid #2f80ed;");

    QVBoxLayout *layoutStoS = new QVBoxLayout(sendToSender);
    layoutStoS->setSpacing(2);
    layoutStoS->setContentsMargins(0,0,0,0);

    QCheckBox *checklayoutStoS = new QCheckBox;
    connect(checklayoutStoS,
            &QCheckBox::clicked,
            this,
            [=](bool checked)
    {
        for(ClientRow *client : clients)
        {
            client->sendToSender->setChecked(checked);
        }

    });

    layoutStoS->addWidget(checklayoutStoS, 0, Qt::AlignCenter);

    QLabel *labellayoutStoS = new QLabel("Göndericiye\nGönder");
    labellayoutStoS->setAlignment(Qt::AlignCenter);
    layoutStoS->addWidget(labellayoutStoS);
       // =====================================================
    // ROLESENDtoReceiver
    // =====================================================

    QWidget *sendToReceiver = new QWidget;
    sendToReceiver->setFixedWidth(w/2);
    sendToReceiver->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //sendToSender->setStyleSheet("color:#1f2a37;border: 1px solid #2f80ed;");

    QVBoxLayout *layoutStoR = new QVBoxLayout(sendToReceiver);
    layoutStoR->setSpacing(2);
    layoutStoR->setContentsMargins(0,0,0,0);

    QCheckBox *checklayoutStoR = new QCheckBox;
    connect(checklayoutStoR,
            &QCheckBox::clicked,
            this,
            [=](bool checked)
    {
        for(ClientRow *client : clients)
        {
            client->sendToReceiver->setChecked(checked);
        }
    });

    layoutStoR->addWidget(checklayoutStoR, 0, Qt::AlignCenter);

    QLabel *labellayoutStoR = new QLabel("Alıcıya\n Gönder");
    labellayoutStoR->setAlignment(Qt::AlignCenter);
    layoutStoR->addWidget(labellayoutStoR);
      // =====================================================
    // ROLERECEIVER
    // =====================================================
    QWidget *receiver = new QWidget;
    receiver->setFixedWidth(w/2);
    receiver->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //receiver->setStyleSheet("color:#1f2a37;border: 1px solid #2f80ed;");

    QVBoxLayout *layoutR = new QVBoxLayout(receiver);
    layoutR->setSpacing(2);
    layoutR->setContentsMargins(0,0,0,0);

    QCheckBox *checkR = new QCheckBox;
    connect(checkR,
            &QCheckBox::clicked,
            this,
            [=](bool checked)
    {
        for(ClientRow *client : clients)
        {
            client->receiver->setChecked(checked);
        }
    });

    layoutR->addWidget(checkR, 0, Qt::AlignCenter);

    QLabel *labelR = new QLabel("Dosya\n Getir");
    labelR->setAlignment(Qt::AlignCenter);
    layoutR->addWidget(labelR);
    // =====================================================
    // ROLEGETTOOTHER
    // =====================================================

    QWidget *getToOther = new QWidget;
    getToOther->setFixedWidth(w*0.8);
    getToOther->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //getToOther->setStyleSheet("color:#1f2a37;border: 1px solid #2f80ed;");

    QVBoxLayout *layoutG = new QVBoxLayout(getToOther);
    layoutG->setSpacing(2);
    layoutG->setContentsMargins(0,0,0,0);

    QCheckBox *checkG = new QCheckBox;
    connect(checkG,
            &QCheckBox::clicked,
            this,
            [=](bool checked)
    {
        for(ClientRow *client : clients)
        {
            client->getToOther->setChecked(checked);
        }
    });

    layoutG->addWidget(checkG, 0, Qt::AlignCenter);

    QLabel *labelG = new QLabel("Başkası\n Göndersin");
    labelG->setAlignment(Qt::AlignCenter);
    layoutG->addWidget(labelG);
    // =====================================================
    // ROLEGETTOOTHER
    // =====================================================
    QLabel *btnlbl = new QLabel("İzinleri Kaydet");
    btnlbl->setFixedWidth(w);
    btnlbl->setStyleSheet("color:#1f2a37;font-weight:800;");
    // =====================================================
    // PROGRESS
    // =====================================================
    client->progresBar = new QProgressBar;
    client->progresBar->setRange(0,100);
    client->progresBar->setValue(0);
    client->progresBar->setFormat("Kopyalama Süreci");
    client->progresBar->setMinimumWidth(w*1.5);
    client->progresBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    client->progresBar->setStyleSheet(QString(R"(
    QProgressBar {
        border: none;
        color:#1f2a37;
        font-weight:800;
        text-align: center;
        font-size:%1px;
        border-radius: 0px;
        background-color: transparent;
    }
    )").arg(font));

    // =====================================================
    // SPEED
    // =====================================================
    client->speedLabel = new QLabel("MB/s");
    client->speedLabel->setStyleSheet("color:#1f2a37;font-weight:800;");
    client->speedLabel->setAlignment(Qt::AlignCenter);
    client->speedLabel->setFixedWidth(h*1.4);
    // =====================================================
    // TIME
    // =====================================================
    client->timeLabel = new QLabel("Zaman");
    client->timeLabel->setAlignment(Qt::AlignCenter);
    client->timeLabel->setFixedWidth(h*0.9);
    client->timeLabel->setStyleSheet("color:#1f2a37;font-weight:800;");
    // =====================================================
    // STOPBUTTON
    // =====================================================
    QToolButton *stopButton= new QToolButton;
    stopButton->setIcon(QIcon(":/icons/stop.svg"));
    stopButton->setToolTip("Tüm Transferleri Durdur");
    stopButton->setFixedWidth(w/5);
    stopButton->setStyleSheet("Text-align:center");
    stopButton->setAutoRaise(true);
    stopButton->setStyleSheet(R"(

    QToolButton {
        color: #6b7a90;
        padding: 0px;
        border: 1px solid transparent;
        border-radius: 4px;
        background-color: transparent;
    }

    QToolButton:hover {
        background-color: #256fd1;
        border: 1px solid #cfe2ff;
    }

    QToolButton:pressed {
        background-color: #d16f25;
        border: 1px solid #b7d4ff;
    }

    )");
    connect(stopButton,&QToolButton::clicked,this,[=](){
        sendStopTransferAll();
    });
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
            ///qDebug()<<checked;
            if(checked)
                selectAllButtonClicked();
                else
                unSelectAllButtonClicked();
            });

    // =====================================================
    // ADD
    // =====================================================

    layout->addWidget(client->ip);
    layout->addWidget(client->hostName);
    layout->addWidget(client->role);
    if(getRole().root){
    layout->addWidget(sendToSender);
    layout->addWidget(sendToReceiver);
    layout->addWidget(receiver);
    layout->addWidget(getToOther);
    layout->addWidget(btnlbl);
    }
    layout->addWidget(client->progresBar,1);
    layout->addWidget(client->speedLabel);
    layout->addWidget(client->timeLabel);
    layout->addWidget(stopButton);
    layout->addWidget(client->select);
    //listLayout->insertWidget(listLayout->count()-1,row);
    //clients[ip] = client;
    client->row = row;
    return row;

}

#endif // UPDATE_H
