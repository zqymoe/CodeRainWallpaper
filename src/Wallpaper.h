#pragma once

#include <QWidget>
#include <QPixmap>
#include <QVector>

class Wallpaper : public QWidget
{
    Q_OBJECT
private:
    QPixmap pixmap;
    QString string;
    QVector<int> vector;

public:
    Wallpaper();
    ~Wallpaper();
    void timerEvent(QTimerEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};