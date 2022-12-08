import QtQuick

Item {
    id: control

    property real tiltAngle: 45
    property real tiltMaxAngle: 60

    property real tiltRotation: 45

    property real indicatorWidth: 4
    property real indicatorRadius: Math.min(width, height) * .9 / 2

    property color borderColor: tiltAngle < tiltMaxAngle
                                ? palette.dark
                                : "red"

    property color indicatorColor: tiltAngle < tiltMaxAngle
                                   ? palette.text
                                   : "red"

    Rectangle {
        anchors.centerIn: parent

        border.color: borderColor
        border.width: 1
        radius: indicatorRadius
        width: 2 * indicatorRadius
        height: 2 * indicatorRadius
        color: "transparent"
    }

    Rectangle {
        width: indicatorRadius * tiltAngle / tiltMaxAngle
        height: indicatorWidth

        x: control.width / 2
        y: (control.height - indicatorWidth ) / 2
        color: indicatorColor

        transform: Rotation {
            origin.x: 0
            origin.y: indicatorWidth / 2
            angle: 360 - 90 - tiltRotation
        }
    }
}
