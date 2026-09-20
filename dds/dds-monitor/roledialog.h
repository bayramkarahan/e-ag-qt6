#ifndef ROLEDIALOG_H
#define ROLEDIALOG_H

#include <QDialog>
#include<QTextStream>
#include<QDebug>
#include<Database.h>
#include<type.h>
class QLineEdit;
class QToolButton;
class QLabel;
class QCheckBox;

class RoleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RoleDialog(int w,int h,QWidget *parent = nullptr);

private slots:
    void saveRole();
private:
    QLineEdit *roleEdit;
    QCheckBox *sendToSenderCB;
    QCheckBox *sendToReceiverCB;
    QCheckBox *receiverCB;
    QCheckBox *getToOtherCB;
    QCheckBox *rootCB;
    QToolButton *saveButton;
    QToolButton *cancelButton;
    QString localDir;
};

#endif // ROLEDIALOG_H
