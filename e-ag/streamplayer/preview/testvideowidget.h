#ifndef TESTVIDEOWIDGET_H
#define TESTVIDEOWIDGET_H

#include <QWidget>
#include <QImage>
#include <QMutex>

class TestVideoWidget : public QWidget
{
    Q_OBJECT

public:

    explicit TestVideoWidget(QWidget *parent=nullptr);

    void setImage(const QImage &image);

protected:

    void paintEvent(QPaintEvent *) override;

private:

    QImage m_image;

    QMutex m_mutex;
};

#endif
