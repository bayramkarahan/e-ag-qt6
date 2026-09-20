#ifndef MONITORCLIENT_H
#define MONITORCLIENT_H

#include <QDialog>
#include <QMap>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include<QDebug>
#include<QToolButton>
#include<QCheckBox>
#include <QTimer>
#include<QDateTime>
#include<QGraphicsDropShadowEffect>
#include<QRadioButton>
#include<QButtonGroup>
#include<QVBoxLayout>
#include<QGroupBox>
#include<QMenu>
#include<QPushButton>
#include<QWidgetAction>
#include<type.h>
#include<QLineEdit>
#include<QFileDialog>
#include<QFileInfo>
#include<QInputDialog>
#include<roledialog.h>
#include<QProcess>
#include<userprivilegehelper.h>

class QScrollArea;


class MonitorClient : public QDialog
{
    Q_OBJECT

public:
    explicit MonitorClient(QWidget *parent = nullptr);
    QSet<QString> seen;
    QProgressBar *totalBar = nullptr;
    QLineEdit *filePathLE;
    void updateProgress(quint64 transferId,const QString &ip, qint64 received, qint64 total, int percent, QString clientHostName, QString role, bool rolesendToSender, bool rolesendToReceiver, bool rolereceiver, bool rolegetToOther);
    void allProgressStart(qint64 totalSize);
    void updateTotalProgressBars();
    void updateTotalBarText(const QString & text );

    void markDone(const QString &ip);
    void markDebInstallStart(const QString &ip);
    void markDebInstallDone(const QString &ip,const QString &status);
    void markScriptInstallStart(const QString &ip);
    void markScriptInstallDone(const QString &ip,const QString &status);
    void progressClientEdit(const QString &ip,const QString &status);
    void sendStopTransfer(quint64 transferId, QString ip);
    void sendStopTransferAll();
public slots:
    void sendRoleStatus(QString ip);
    void sendRoleStatusAll();
    void receivedMonitorDiscoveryReply(QByteArray d, QHostAddress sender);
    void receivedClientAdd(QByteArray d, QHostAddress sender);
    void receivedMonitorProgress(QByteArray d, QHostAddress sender);
    void receivedDebStart(QByteArray d, QHostAddress sender);
    void receivedDebDone(QByteArray d, QHostAddress sender);
    void receivedScriptStart(QByteArray d, QHostAddress sender);
    void receivedScriptDone(QByteArray d, QHostAddress sender);

private slots:
    bool isUserSpecialDirectory(const QString &path);
    QScrollArea *createListeWidget();
    QWidget *createPathWidget();
    QWidget *createButtonWidget();
    void about();
    void updConnect();
    void onDiscoveryButtonClicked();
    void selectAllButtonClicked();
    void unSelectAllButtonClicked();
    void getFileButtonClicked(SourceLocation sourceLocation);
    RoleInfo getRole();
    void sortClients();
    QFrame *createHeader();
    QString  getDesktopPathFromHome(const QString &home);
    void sendMonitorDiscovery();
     void processPendingDatagrams();
     void sendSenderData(QString destPath, TargetLocation targetLocation);
     void sendReceiverData(QString fileName, SourceType sourceType, SourceLocation sourceLocation, TargetLocation targetLocation);
signals:

    void updateSelectClients();
    void sendFileTransferAllDone(qint64 averageTime);
    void updatePath(QString path);
    void getFile(QString myfile,SourceType mySourceType);

private:
    //QMap<QString, rowClient*> progressBars;
    QVBoxLayout *listLayout = nullptr;
    QScrollArea *scroll= nullptr;
    QWidget *pathWidget = nullptr;
    QWidget *buttonWidget = nullptr;
    QWidget *container = nullptr;
    QWidget *listHeaderWidget = nullptr;
    QFrame *header= nullptr;
    QToolButton *sendFileButton;
    QToolButton *discoveryButton;
    QToolButton *getFileButton;
    QToolButton *sendRoleStatusAllButton;
    int h=50;
    int w=100;
    QString font="10";
    QMenu *sendMenu();
    QMenu *getMenu();
    QString localDir;
    RoleInfo mainrolinfo;
    QLabel *infoLabel;
    QUdpSocket getUdpSocket;
    QVBoxLayout *mainLayout ;
};

#endif
