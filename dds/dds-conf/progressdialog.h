#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

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
class QScrollArea;

struct ClientRow
{
    QLabel *ip;
    QLabel *hostName;
    QProgressBar *progresBar;
    QLabel *speedLabel;
    QLabel *timeLabel;
    QCheckBox *select;
    qint64 diffTime;
    qint64 lastTime;
    qint64 startTime;
    qint64 totalFileSize = 0;
};

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    QMap<QString, ClientRow*> clients;
    QProgressBar *totalBar = nullptr;
    QLineEdit *filePathLE;
    void updateProgress(const QString &ip, qint64 received, qint64 total, int percent, QString clientHostName);
    void allProgressStart(qint64 totalSize);
    void updateTotalProgressBars();
    void updateTotalBarText(const QString & text );

    void markDone(const QString &ip);
    void markDebInstallStart(const QString &ip);
    void markDebInstallDone(const QString &ip,const QString &status);
    void markScriptInstallStart(const QString &ip);
    void markScriptInstallDone(const QString &ip,const QString &status);
    void progressClientEdit(const QString &ip,const QString &status);


private slots:
    void onDiscoveryButtonClicked();
    void selectAllButtonClicked();
    void unSelectAllButtonClicked();
    void onGetFileButtonClicked();
signals:
    void startTransfer(QString destPath,TransferLocation type);
    void sendDiscovery();
    void updateSelectClients();
    void sendFileTransferAllDone(qint64 averageTime);
    void updatePath(QString path);
    void getFile(QString myfile,SourceType mySourceType);
private:
    //QMap<QString, rowClient*> progressBars;


    QVBoxLayout *listLayout = nullptr;
    QScrollArea *scroll= nullptr;
    QWidget *container = nullptr;
    QWidget *pathWidget = nullptr;
    QWidget *buttonWidget = nullptr;
    QToolButton *sendFileButton;
    QToolButton *discoveryButton;
    QToolButton *selectAllButton;
    QToolButton *unSelectAllButton;
    QToolButton *getFileButton;
    int h=50;
    int w=100;
    QString font="10";
    QMenu *sendMenu();
};

#endif
