#ifndef PATHWIDGET_H
#define PATHWIDGET_H
QWidget *MonitorClient::createPathWidget()
{
QWidget *pw = new QWidget;
pw->resize(w*6,h*0.5);
QHBoxLayout *pathLayout = new QHBoxLayout(pw);
pathLayout->setContentsMargins(0,0,0,0);
pathLayout->setSpacing(0);

QLabel *filePathLabel=new QLabel("Dosya Konumu");
filePathLabel->setFixedSize(w,h*0.5);
filePathLabel->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");

filePathLE = new QLineEdit();
///filePathLE->setText("./aaa.txt");
filePathLE->setMinimumWidth(w * 4);
//filePathLE->resize(w*4.5,h*0.5);
//filePathLE->setFixedHeight(h*0.5);
filePathLE->setSizePolicy(
    QSizePolicy::Expanding,
    QSizePolicy::Fixed
    );
filePathLE->setStyleSheet("font-size:"+QString::number(font.toInt()+2)+"px;");

QToolButton *fileSelectButton=new QToolButton();
fileSelectButton->setFixedSize(w/2,h*0.5);
fileSelectButton->setObjectName("fileSelectButton");
fileSelectButton->setAutoRaise(true);
//fileSelectButton->setAutoFillBackground(true);
fileSelectButton->setText(tr("..."));
fileSelectButton->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");
fileSelectButton->setIcon(QIcon(":/icons/file.svg"));
fileSelectButton->setIconSize(QSize(w/2,h*0.4));
fileSelectButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

connect(fileSelectButton, &QToolButton::clicked, [=]() {
    UserPrivilegeHelper helper;
    SessionInfo userInfo = helper.getActiveSessionInfo();
    QString desktop=getDesktopPathFromHome(userInfo.home);

    //qDebug()<<"desktop: "<<desktop;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Dosya Seç"),desktop, tr("Files (*)"));
    filePathLE->setText(fileName);
    updatePath(fileName);

});
QToolButton *directorySelectButton=new QToolButton();
directorySelectButton->setFixedSize(w/2,h*0.5);
directorySelectButton->setObjectName("directorySelectButton");
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

QToolButton *aboutButton=new QToolButton();
aboutButton->setFixedSize(w/3,h*0.5);
aboutButton->setAutoRaise(true);
aboutButton->setObjectName("aboutButton");
//aboutButton->setAutoFillBackground(true);
aboutButton->setText(tr("..."));
aboutButton->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");
aboutButton->setIcon(QIcon(":/icons/about.svg"));
aboutButton->setIconSize(QSize(w,h));
aboutButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

connect(aboutButton, &QToolButton::clicked, [=]() {
    about();
});
pathLayout->addWidget(filePathLabel);
pathLayout->addWidget(filePathLE);
QLabel *boslukLabel=new QLabel("");boslukLabel->setFixedWidth(h/2);
pathLayout->addWidget(boslukLabel);
pathLayout->addWidget(fileSelectButton);
pathLayout->addWidget(directorySelectButton);
pathLayout->addWidget(boslukLabel);
pathLayout->addWidget(aboutButton);
return pw;
}
#endif // PATHWIDGET_H
