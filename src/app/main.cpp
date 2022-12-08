#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName("Ioan Calin");
    QGuiApplication::setOrganizationDomain("com.gitlab.iborco");
    QGuiApplication::setApplicationName("pendoctor");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    qDebug() << "Settings file:" << QSettings().fileName();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
