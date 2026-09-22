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

    DatabaseHelper db(localDir+"group.json");
    QJsonArray dizi=db.Oku();
    int sr=0;



 /*   DatabaseHelper db(localDir + "persist.json");
    QJsonArray dizi = db.Oku();
    PcData::groupListe.clear();
    if (dizi.count() > 0) {
        for (const QJsonValue &item : dizi) {
            QJsonObject veri = item.toObject();
            bool findState = false;
            for (const groupList &itemliste : PcData::groupListe) {
                if (itemliste.groupName == veri.value("groupname").toString()) {
                    findState = true;
                    break;
                }
            }
            if (!findState)
            {
                PcData::groupListe.append({true, veri.value("groupname").toString()});

            }

        }
    }

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

            pcListeGuncelleSlot("groupListChange");

        });

        rowLayout->addWidget(check);
        rowLayout->addWidget(label);
        rowLayout->addStretch();
        listLayout->addWidget(row);
    }
    /*
    for (int i = 0; i < PcData::groupListe.size(); ++i) {
        groupList &grp = PcData::groupListe[i];

        QWidget *row = new QWidget();
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(2, 0, 2, 0);
        rowLayout->setSpacing(5);

        QCheckBox *check = new QCheckBox();
        check->setChecked(grp.groupSelect);
        QLabel *label = new QLabel(grp.groupName);
        check->setObjectName("checkWidget");
        check->setStyleSheet("#checkWidget { border: 1px solid #e1e1e1; }");
        connect(check, &QCheckBox::toggled, sor, [this, i, check,grp]() {
            PcData::groupListe[i].groupSelect = check->isChecked();
            groupListChange(PcData::groupListe);
        });

        rowLayout->addWidget(check);
        rowLayout->addWidget(label);
        rowLayout->addStretch();
        listLayout->addWidget(row);
    }*/

    listLayout->addStretch();
    container->setLayout(listLayout);
    scroll->setWidget(container);
    mainLayout->addWidget(scroll);

    sor->setLayout(mainLayout);
    return sor;
}
/*
void MainWindow::groupListChange(QString groupName,bool groupSelect)
{
    pcListeGuncelleSlot("groupListChange");

    DatabaseHelper db(localDir+"group.json");
    for (const groupList &g : PcData::groupListe) {
            qDebug()<<"gruplar"<<g.groupSelect << g.groupName;
            QJsonObject obj;
            obj["groupName"] = g.groupName;
            obj["groupSelect"] = g.groupSelect;
            db.Sil("groupName",g.groupName);
            db.Ekle(obj);
            ///qDebug() << "group yazıldı:" << g.groupName<<g.groupSelect;

    }
}
*/

#endif // GROUPWIDGET_H
