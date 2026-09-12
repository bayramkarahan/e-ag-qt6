#ifndef MASTERTIME_H
#define MASTERTIME_H

#include <QObject>
#include <QElapsedTimer>

class MasterTime : public QObject
{
    Q_OBJECT

public:
    explicit MasterTime(QObject *parent = nullptr);

    void start();

    void reset();

    qint64 nowUs() const;

private:
    QElapsedTimer m_timer;
};

#endif
