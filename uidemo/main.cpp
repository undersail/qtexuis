#include "mainwindow.h"

#include <QApplication>

#include <QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // window style: windows, motif, cde, plastique, windowsxp or macintosh
    QApplication::setStyle("plastique");
    // set form style with css
    a.setStyleSheet(QString("QWidget{background-color:#e5e5e5; border-style.:flat;}"));

    // …Ë÷√÷–Œƒ±‡¬Î
    QString sPath = a.applicationDirPath();
    sPath += QString("/plugins");
    a.addLibraryPath(sPath);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    MainWindow w;
    w.show();
    return a.exec();
}
