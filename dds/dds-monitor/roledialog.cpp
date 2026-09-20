#include "roledialog.h"
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include<QCheckBox>

RoleDialog::RoleDialog(int w, int h, QWidget *parent)
    : QDialog(parent)
{
    localDir="/usr/share/dds/";
    setFixedSize(w*3,h*3.5);
    setWindowTitle("DDS Ayarlar");

    QLabel *roleLabel = new QLabel("Paylaşım Rolü:");
    QRadioButton *gonderici = new QRadioButton("Gönderici");
    QRadioButton *alici = new QRadioButton("Alıcı");
    gonderici->setStyleSheet("color:#1f2a37;font-weight:800;");
    alici->setStyleSheet("color:#1f2a37;font-weight:800;");
    QButtonGroup *group = new QButtonGroup(this);

    group->addButton(gonderici, 0);
    group->addButton(alici, 1);

    roleEdit = new QLineEdit;
    connect(group, &QButtonGroup::idClicked,
            this, [this](int id)
    {
        //qDebug() << "Seçilen:" << id;
        if(id==0) roleEdit->setText("sender");
        if(id==1) roleEdit->setText("receiver");
    });

    QLabel *sendToSenderLabel = new QLabel("<b>Göndericiye</b>  Gönder:");
    sendToSenderCB=new QCheckBox;
    sendToSenderCB->setStyleSheet(R"(

        QCheckBox {
            spacing: 0px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 4px;
            border: 1px solid #cfd8e3;
            background-color: white;
        }

        QCheckBox::indicator:checked {
            background-color: #2f80ed;
            border: 1px solid #2f80ed;
        }

        )");
    QLabel *sendToReceiverLabel = new QLabel("<b>Alıcıya</b> Gönder:");
    sendToReceiverCB=new QCheckBox;
    sendToReceiverCB->setStyleSheet(R"(

        QCheckBox {
            spacing: 0px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 4px;
            border: 1px solid #cfd8e3;
            background-color: white;
        }

        QCheckBox::indicator:checked {
            background-color: #2f80ed;
            border: 1px solid #2f80ed;
        }

        )");
    QLabel *receiverLabel = new QLabel("Dosya Getir:");
    receiverCB=new QCheckBox;
    receiverCB->setStyleSheet(R"(

        QCheckBox {
            spacing: 0px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 4px;
            border: 1px solid #cfd8e3;
            background-color: white;
        }

        QCheckBox::indicator:checked {
            background-color: #2f80ed;
            border: 1px solid #2f80ed;
        }

        )");

    QLabel *getToOtherLabel = new QLabel("Başkası Göndersin:");
    getToOtherCB=new QCheckBox;
    getToOtherCB->setStyleSheet(R"(

        QCheckBox {
            spacing: 0px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 4px;
            border: 1px solid #cfd8e3;
            background-color: white;
        }

        QCheckBox::indicator:checked {
            background-color: #2f80ed;
            border: 1px solid #2f80ed;
        }

        )");
    QLabel *rootLabel = new QLabel("Diğerlerinin Ayarlarını Yönet:");
    rootCB=new QCheckBox;
    rootCB->setStyleSheet(R"(

        QCheckBox {
            spacing: 0px;
        }

        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 4px;
            border: 1px solid #cfd8e3;
            background-color: white;
        }

        QCheckBox::indicator:checked {
            background-color: #2f80ed;
            border: 1px solid #2f80ed;
        }

        )");

    DatabaseHelper *db=new DatabaseHelper(localDir+"dds.json");
    QJsonArray dizi=db->Oku();
    if(dizi.count()>0)
    {
        QJsonValue item=dizi.first();
        QJsonObject veri=item.toObject();
        roleEdit->setText(veri["role"].toString());

        if(roleEdit->text()=="sender")
            gonderici->setChecked(true);

        if(roleEdit->text()=="receiver")
            alici->setChecked(true);

        sendToSenderCB->setChecked(veri["sendToSender"].toBool());
        sendToReceiverCB->setChecked(veri["sendToReceiver"].toBool());
        receiverCB->setChecked(veri["receiver"].toBool());
        getToOtherCB->setChecked(veri["getToOther"].toBool());
        rootCB->setChecked(veri["root"].toBool());

    }else
    {
        QJsonObject veri;
        veri["role"]="sender";

        veri["sender"]=true;
        veri["receiver"]=true;
        db->TekKayitYaz(veri);
        roleEdit->setText(veri["role"].toString());
        if(roleEdit->text()=="sender")
            gonderici->setChecked(true);

        if(roleEdit->text()=="receiver")
            alici->setChecked(true);
        sendToSenderCB->setChecked(veri["sendToSender"].toBool());
        sendToReceiverCB->setChecked(veri["sendToReceiver"].toBool());
        receiverCB->setChecked(veri["receiver"].toBool());
        getToOtherCB->setChecked(veri["getToOther"].toBool());
        rootCB->setChecked(veri["root"].toBool());

    }


    saveButton = new QToolButton;
    saveButton->setText("Kaydet");
    saveButton->setObjectName("saveButton");
    cancelButton = new QToolButton;
    cancelButton->setText("İptal");

    connect(saveButton, &QToolButton::clicked, this, &RoleDialog::saveRole);
    connect(cancelButton, &QToolButton::clicked, this, &RoleDialog::reject);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(saveButton);
    btnLayout->addWidget(cancelButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(roleLabel,1,0,1,1);
    ///mainLayout->addWidget(roleEdit,1,1,1,1);
    mainLayout->addWidget(gonderici,1,1,1,1);
    mainLayout->addWidget(alici,1,2,1,1);

    mainLayout->addWidget(sendToSenderLabel,3,0,1,1);
    mainLayout->addWidget(sendToSenderCB,3,1,1,1);
    mainLayout->addWidget(sendToReceiverLabel,4,0,1,1);
    mainLayout->addWidget(sendToReceiverCB,4,1,1,1);

    mainLayout->addWidget(receiverLabel,6,0,1,1);
    mainLayout->addWidget(receiverCB,6,1,1,1);
    mainLayout->addWidget(getToOtherLabel,7,0,1,1);
    mainLayout->addWidget(getToOtherCB,7,1,1,1);
    mainLayout->addWidget(rootLabel,8,0,1,1);
    mainLayout->addWidget(rootCB,8,1,1,1);

    mainLayout->addLayout(btnLayout,9,0,1,2,Qt::AlignCenter);

    setLayout(mainLayout);

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

    QToolButton#saveButton {
        background-color: #256fd1;
        color: white;
        border: none;
    }

    QToolButton#saveButton:hover {
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

}

void RoleDialog::saveRole()
{
    RoleInfo ri;

    ri.role = roleEdit->text().trimmed();
    ri.sendToSender=sendToSenderCB->isChecked();
    ri.sendToReceiver=sendToReceiverCB->isChecked();
    ri.receiver=receiverCB->isChecked();
    ri.getToOther=getToOtherCB->isChecked();
    ri.root=rootCB->isChecked();

    log(QString("SEND MONITOR_SETTINGS"));
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)MONITOR_SETTINGS;
    stream << ri.role;
    stream << ri.sendToSender;
    stream << ri.sendToReceiver;
    stream << ri.receiver;
    stream << ri.getToOther;
    stream << ri.root;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);
     accept();
}
