#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H
QWidget *MonitorClient::createButtonWidget()
{
    QWidget *bw = new QWidget;
    //buttonWidget = new QWidget;
    bw->resize(w*6,h*0.5);
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout(bw);
    buttonWidgetLayout->setContentsMargins(0,0,0,0);
    buttonWidgetLayout->setSpacing(0);

    QToolButton *settingButton= new QToolButton;
    settingButton->setFixedSize(w*0.7,h);
    settingButton->setIconSize(QSize(w*0.4,h*0.4));
    settingButton->setStyleSheet("Text-align:center");
    settingButton->setIcon(QIcon(":/icons/settings.svg"));
    settingButton->setAutoRaise(true);
    settingButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    settingButton->setText("Ayarlar");

    connect(settingButton, &QPushButton::clicked, [=]() {
        QProcess p;
        p.start("pkexec", QStringList() << "/usr/bin/true");
        p.waitForFinished();

        if (p.exitStatus() == QProcess::NormalExit &&
                p.exitCode() == 0)
        {
            RoleDialog *dlg=new RoleDialog(w*1.5,h*1.5);
            if (dlg->exec() == QDialog::Accepted)
            {
                qDebug() << "dds rolü değiştirildi.";
            }

            QTimer::singleShot(500, [this]()
            {


                mainrolinfo = getRole();

                infoLabel->setText(
                    QString("Rol: %1 -- Ip: %2")
                        .arg(mainrolinfo.role == "sender" ? "Gönderici" : "Alıcı")
                        .arg(getLocalIp())
                );
                if(mainrolinfo.sendToSender||mainrolinfo.sendToReceiver)
                    sendFileButton->setVisible(true);
                else
                    sendFileButton->setVisible(false);

                if(mainrolinfo.receiver)
                    getFileButton->setVisible(true);
                else
                    getFileButton->setVisible(false);

                // Listeyi Yenile
                onDiscoveryButtonClicked();

                // ==================================================
                // Header Update
                // ==================================================
                mainLayout->removeWidget(header);
                header->deleteLater();
                header = createHeader();
                mainLayout->insertWidget(mainLayout->indexOf(scroll),header);

            });

        }
        else
        {
            qDebug() << "StdErr:" << p.readAllStandardError();
            QMessageBox::critical(this,
                                  "Hata",
                                  "Kimlik doğrulama başarısız.");
        }



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
            &MonitorClient::onDiscoveryButtonClicked);


    sendRoleStatusAllButton = new QToolButton;
    sendRoleStatusAllButton->setText("Tüm İzinleri Kaydet");
    sendRoleStatusAllButton->setFixedSize(w*1.5,h);
    sendRoleStatusAllButton->setIcon(QIcon(":/icons/save.svg"));
    sendRoleStatusAllButton->setIconSize(QSize(w*0.4,h*0.4));
    sendRoleStatusAllButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(sendRoleStatusAllButton, &QPushButton::clicked, [=]() {
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
            sendRoleStatusAll();
        }
        else if(msgBox.clickedButton() == cancelButton)
        {
            return;
        }
    });


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

    getFileButton = new QToolButton;
    getFileButton->setText("Getir");
    getFileButton->setFixedSize(w*0.7,h);
    getFileButton->setIcon(QIcon(":/icons/getdesktop.svg"));
    getFileButton->setIconSize(QSize(w*0.5,h*0.4));
    getFileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //  connect(getFileButton,&QToolButton::clicked, this,
    //         &MonitorClient::onGetFileButtonClicked);
    getFileButton->setMenu(getMenu());
    getFileButton->setPopupMode(QToolButton::InstantPopup);

    infoLabel->setText(
        QString("Rol: %1 -- Ip: %2")
            .arg(mainrolinfo.role == "sender" ? "Gönderici" : "Alıcı")
            .arg(getLocalIp())
    );
    infoLabel->setFixedHeight(h);
    infoLabel->setMinimumWidth(w*1.5);
    infoLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    infoLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    int fn=font.toInt();
    fn=fn*1.5;
    infoLabel->setStyleSheet(QString(R"(
    QLabel {
        border: none;
        color:#ff0000;
        font-weight:800;
        text-align: center;
        font-size:%1px;
        border-radius: 0px;
        background-color: transparent;
    } )").arg(QString::number(fn)));

    buttonWidgetLayout->addWidget(settingButton);
    buttonWidgetLayout->addWidget(discoveryButton);
    buttonWidgetLayout->addWidget(sendRoleStatusAllButton);

    ///buttonWidgetLayout->addStretch();
    buttonWidgetLayout->addWidget(infoLabel);
    buttonWidgetLayout->addWidget(getFileButton);
    buttonWidgetLayout->addWidget(sendFileButton);

    return bw;
}
#endif // BUTTONWIDGET_H
