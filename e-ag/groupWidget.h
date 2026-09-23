#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H
#include<Database.h>

QWidget* MainWindow::groupWidget(QWidget *targetWidget)
{
    int e = en;
    int b = boy;
    int yukseklik = e * 12;

    localDir = "/usr/share/e-ag/";
#if defined(Q_OS_WIN)
    localDir = "c:/e-ag/";
#elif defined(Q_OS_LINUX)
    localDir = "/usr/share/e-ag/";
#endif


    int sr=0;
    DatabaseHelper dbpersist(localDir + "persist.json");
    /*****************kullanılmayan gruplar siliyor***********************/
    DatabaseHelper dbgroup(localDir + "group.json");
    QJsonArray dizigroup = dbgroup.Oku();
    PcData::groupListe.clear();
    if (dizigroup.count() > 0) {
        for (const QJsonValue &itemgroup : dizigroup) {
            QJsonObject verigroup = itemgroup.toObject();
            //qDebug()<<"gruplar1"<<verigroup.value("groupName").toString();
            QJsonArray dizipersist = dbpersist.Ara("groupname",verigroup.value("groupName"));
            if (dizipersist.count() == 0) {
                //qDebug()<<"silinecek grup:"<<verigroup.value("groupName").toString();
                  dbgroup.Sil("groupName",verigroup.value("groupName"));
                groupWidget(targetWidget);
            }else
            {
                if(verigroup.value("groupSelect").toBool()){
                    //qDebug()<<"seçili gruplar"<<verigroup.value("groupName").toString();
                    PcData::groupListe.append({true, verigroup.value("groupName").toString()});
                }
            }
        }
    }
    qDebug()<<"seçili gruplar sayısı "<<PcData::groupListe.count();

    /***************************************************************/


    QWidget *sor = targetWidget;
    if (!sor) {
        sor = new QWidget();
        sor->setObjectName("groupWidget");
        sor->setStyleSheet("#groupWidget { border: 1px solid #e1e1e1; }");
        sor->setFixedSize(yukseklik * 1.4, boy * 14);
    } else {
        // 🔹 Eski içeriği temizle
        QLayout *oldLayout = sor->layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }
    }

    // 🔹 Yeni layout ve içerik
    QVBoxLayout *mainLayout = new QVBoxLayout(sor);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QLabel *headerLabel = new QLabel(tr("Gruplar"));
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("font-weight:bold;font-size:9px;color:#333;");
    mainLayout->addWidget(headerLabel);

    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background:transparent;");

    QWidget *container = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(container);
    listLayout->setContentsMargins(2, 2, 2, 2);
    listLayout->setSpacing(2);

    /***************************************************/
    DatabaseHelper db(localDir+"group.json");
    QJsonArray dizi=db.Oku();

    for (const QJsonValue &item : dizi) {
        QJsonObject veri=item.toObject();
        bool groupSelect=veri.value("groupSelect").toBool();
        QString groupName=veri.value("groupName").toString();


        QWidget *row = new QWidget();
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(2, 0, 2, 0);
        rowLayout->setSpacing(5);

        QCheckBox *check = new QCheckBox();
        check->setChecked(groupSelect);
        QLabel *label = new QLabel(groupName);
        check->setObjectName("checkWidget");
        check->setStyleSheet("#checkWidget { border: 1px solid #e1e1e1; }");
        connect(check, &QCheckBox::toggled, sor, [this, check,groupName]() {
            DatabaseHelper data(localDir+"group.json");
            QJsonObject obj;
            obj["groupName"] = groupName;
            obj["groupSelect"] =check->isChecked();
            data.Sil("groupName",groupName);
            data.Ekle(obj);
            qDebug() << "group yazıldı:" << groupName<<check->isChecked();
            /******************************************/
            // liste güncelleme
            DatabaseHelper dbgrp(localDir + "group.json");
            QJsonArray dizigrp = dbgrp.Oku();
            PcData::groupListe.clear();
            if (dizigrp.count() > 0) {
                for (const QJsonValue &itemgrp : dizigrp) {
                    QJsonObject verigrp = itemgrp.toObject();

                      if(verigrp.value("groupSelect").toBool()){
                        //qDebug()<<"seçili gruplar"<<verigrp.value("groupName").toString();
                            PcData::groupListe.append({true, verigrp.value("groupName").toString()});
                        }
                    }
                }
            /**********************************************/
            pcListeGuncelleSlot("groupListChange");
        });

        rowLayout->addWidget(check);
        rowLayout->addWidget(label);
        rowLayout->addStretch();
        listLayout->addWidget(row);
    }

    listLayout->addStretch();
    container->setLayout(listLayout);
    scroll->setWidget(container);
    mainLayout->addWidget(scroll);

    sor->setLayout(mainLayout);
    return sor;
}

#endif // GROUPWIDGET_H
