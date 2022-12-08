import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: control

    component DescriptionLabel: Label {
        Layout.alignment: Qt.AlignRight
        font.weight: Font.Light
    }

    component ValueLabel: Label {
        font.weight: Font.Medium
        Layout.fillWidth: true
    }

    component HorizontalLine: Rectangle {
        color: control.palette.dark
        height: 1
        Layout.fillWidth: true
    }

    GridLayout {
        width: parent.width

        columns: 2

        DescriptionLabel { text: qsTr("Device ID:") }
        ValueLabel { text: penInfo.deviceId }

        DescriptionLabel { text: qsTr("Device Type:") }
        ValueLabel { text: penInfo.deviceTypeName }

        DescriptionLabel { text: qsTr("Pointer Type:") }
        ValueLabel { text: penInfo.pointerTypeName }

        DescriptionLabel { text: qsTr("Pressure:") }
        ValueLabel { text: penInfo.pressure }

        DescriptionLabel { text: qsTr("Rotation:") }
        ValueLabel { text: penInfo.rotation }

        DescriptionLabel { text: qsTr("Tangential Pressure:") }
        ValueLabel { text: penInfo.tangentialPressure }

        DescriptionLabel { text: qsTr("X Tilt:") }
        ValueLabel { text: penInfo.xTilt }

        DescriptionLabel { text: qsTr("Y Tilt:") }
        ValueLabel { text: penInfo.yTilt }

        DescriptionLabel { text: qsTr("Z:") }
        ValueLabel { text: penInfo.z }

        HorizontalLine { GridLayout.columnSpan: 2 }

        DescriptionLabel { text: qsTr("Tilt Angle:") }
        ValueLabel { text: penInfo.tiltAngle }

        DescriptionLabel { text: qsTr("Tilt Rotation:") }
        ValueLabel { text: penInfo.tiltRotation }
    }
}
