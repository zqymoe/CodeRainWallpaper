#include "Wallpaper.h"
#include <QPainter>
#include <QScreen>
#include <qmath.h>
#include <QRandomGenerator>
#include <QDebug>
#include <Windows.h>

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
    HWND hDefView = FindWindowEx(hwnd, 0, "SHELLDLL_DefView", 0);
    if (hDefView != 0)
    {
        HWND hWorkerw = FindWindowEx(0, hwnd, "WorkerW", 0);
        ShowWindow(hWorkerw, SW_HIDE);
        return FALSE;
    }
    return TRUE;
}
Wallpaper::Wallpaper() : QWidget()
{
    setWindowFlags(Qt::FramelessWindowHint);
    move(screen()->geometry().x(), screen()->geometry().y());
    resize(screen()->size());
    string = QString("ABCDEFGHIJKLMNOPQRSDUVWXYZ1234567890");
    vector = QVector<int>(qRound(screen()->size().width() / 20.0)).fill(0);
    pixmap = QPixmap(screen()->size());
    pixmap.fill(Qt::white);
    startTimer(40);
    HWND progman = FindWindow("Progman", "Program Manager");
    SendMessageTimeout(progman, 0x52C, 0, 0, 0, 100, 0);
    SetParent((HWND)winId(), progman);
    EnumWindows(EnumWindowsProc, 0);
}

Wallpaper::~Wallpaper()
{
}

void Wallpaper::timerEvent(QTimerEvent *event)
{
    repaint();
}

void Wallpaper::paintEvent(QPaintEvent *event)
{
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.fillRect(pixmap.rect(), QColor(0, 0, 0, 12));
    QFont font("Consolas");
    font.setBold(true);
    font.setPixelSize(24);
    pixmapPainter.setFont(font);
    pixmapPainter.setPen(Qt::green);
    pixmapPainter.setBrush(Qt::green);
    pixmapPainter.setRenderHints(QPainter::HighQualityAntialiasing);
    for (int index = 0; index < vector.length(); index++)
    {
        int item = vector[index];
        pixmapPainter.drawText(
            QRect(index * 20, item, 20, 20),
            Qt::AlignCenter,
            string.at(QRandomGenerator::global()->bounded(string.length())));
        vector[index] = (item > screen()->size().height() ||
                         item > QRandomGenerator::global()->bounded(10000))
                            ? 0
                            : item + 20;
    }
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}
