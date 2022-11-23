#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QInputDevice>
#include <QTabletEvent>

namespace {

const QString& inputDeviceTypeName(QInputDevice::DeviceType deviceType)
{
    static const QString UnknownDeviceType(QObject::tr("Unknown"));
    static const QString MouseDeviceType(QObject::tr("Mouse"));
    static const QString TouchScreenDeviceType(QObject::tr("Touch Screen"));
    static const QString TouchPadDeviceType(QObject::tr("Touch Pad"));
    static const QString PuckDeviceType(QObject::tr("Puck"));
    static const QString StylusDeviceType(QObject::tr("Stylus"));
    static const QString AirbrushDeviceType(QObject::tr("Airbrush"));
    static const QString KeyboardDeviceType(QObject::tr("Keyboard"));

    switch (deviceType) {
    case QInputDevice::DeviceType::Mouse:
        return MouseDeviceType;
    case QInputDevice::DeviceType::TouchScreen:
        return TouchScreenDeviceType;
    case QInputDevice::DeviceType::TouchPad:
        return TouchPadDeviceType;
    case QInputDevice::DeviceType::Puck:
        return PuckDeviceType;
    case QInputDevice::DeviceType::Stylus:
        return StylusDeviceType;
    case QInputDevice::DeviceType::Airbrush:
        return AirbrushDeviceType;
    case QInputDevice::DeviceType::Keyboard:
        return KeyboardDeviceType;
    default:
        return UnknownDeviceType;
    }
}

const QString& pointerTypeName(QPointingDevice::PointerType pointerType)
{
    static const QString UnknownPointerType(QObject::tr("Unknown"));
    static const QString GenericPointerType(QObject::tr("Generic"));
    static const QString FingerPointerType(QObject::tr("Finger"));
    static const QString PenPointerType(QObject::tr("Pen"));
    static const QString EraserPointerType(QObject::tr("Eraser"));
    static const QString CursorPointerType(QObject::tr("Cursor"));

    switch(pointerType) {
    case QPointingDevice::PointerType::Generic:
        return GenericPointerType;
    case QPointingDevice::PointerType::Finger:
        return FingerPointerType;
    case QPointingDevice::PointerType::Pen:
        return PenPointerType;
    case QPointingDevice::PointerType::Eraser:
        return EraserPointerType;
    case QPointingDevice::PointerType::Cursor:
        return CursorPointerType;
    default:
        return UnknownPointerType;
    }
}

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tabletEvent(QTabletEvent *event)
{
    auto device = event->device();
    ui->deviceIdLabel->setText(QString::number(device->systemId()));
    ui->deviceTypeLabel->setText(inputDeviceTypeName(device->type()));
    ui->pointerTypeLabel->setText(pointerTypeName(event->pointerType()));
}

