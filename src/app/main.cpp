/*
    Copyright 2022 by Ioan Calin Borcoman <iborco@gmail.com>

    This file is part of PenInspector.

    PenInspector is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    PenInspector is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    PenInspector. If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "penconfig.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName("Ioan Calin");
    QGuiApplication::setOrganizationDomain("com.gitlab.iborco");
    QGuiApplication::setApplicationName("peninspector");

    qRegisterMetaType<PenConfig>("PenConfig");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    qDebug() << "Settings file:" << QSettings().fileName();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
