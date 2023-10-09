#include <Windows.h>
#include <QApplication>
#include <QSharedMemory>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include "Wallpaper.h"

Wallpaper *wallpaper;
QIcon icon;

void reload()
{
    wallpaper->close();
    wallpaper->deleteLater();
    wallpaper = new Wallpaper;
    wallpaper->setWindowTitle("CodeRainWallpaper");
    wallpaper->setWindowIcon(icon);
    wallpaper->show();
}

int main(int argc, char *argv[])
{
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication app(argc, argv);
    QSharedMemory sharedMemory;
    sharedMemory.setKey("CodeRainWallpaperSharedMemory");
    if (sharedMemory.attach())
    {
        return 0;
    }
    if (sharedMemory.create(1))
    {
        QFileInfo fileInfo(QApplication::applicationFilePath());
        QFileIconProvider fileIcon;
        icon = fileIcon.icon(fileInfo);

        wallpaper = new Wallpaper;
        wallpaper->setWindowTitle("CodeRainWallpaper");
        wallpaper->setWindowIcon(icon);
        wallpaper->show();

        QMenu menu;
        QAction aReload(&menu);
        aReload.setText("重新加载");
        QObject::connect(&aReload, &QAction::triggered, &reload);
        menu.addAction(&aReload);
        QAction aExit(&menu);
        aExit.setText("关闭并退出");
        QObject::connect(&aExit, &QAction::triggered, &app, &QApplication::exit);
        menu.addAction(&aExit);
        menu.setStyleSheet("QMenu{background:rgb(0,0,0);padding:5px;}"
                           "QMenu::item{color:rgb(0,255,0);font-family:黑体;font-size:16px;padding:3px 12px;}"
                           "QMenu::item:selected{background:rgb(32,32,32)}");

        QSystemTrayIcon sti;
        sti.setContextMenu(&menu);
        sti.setIcon(icon);
        sti.show();

        return QApplication::exec();
    }
    return 0;
}
