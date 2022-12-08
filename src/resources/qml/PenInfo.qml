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

    component RealValueLabel: ValueLabel {
        property real value: 0
        text: value ? value.toFixed(4) : "0"
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
        RealValueLabel { value: penInfo.pressure }

        DescriptionLabel { text: qsTr("Rotation:") }
        RealValueLabel { value: penInfo.rotation }

        DescriptionLabel { text: qsTr("Tangential Pressure:") }
        RealValueLabel { value: penInfo.tangentialPressure }

        DescriptionLabel { text: qsTr("X Tilt:") }
        RealValueLabel { value: penInfo.xTilt }

        DescriptionLabel { text: qsTr("Y Tilt:") }
        RealValueLabel { value: penInfo.yTilt }

        DescriptionLabel { text: qsTr("Z:") }
        RealValueLabel { value: penInfo.z }

        HorizontalLine { GridLayout.columnSpan: 2 }

        DescriptionLabel { text: qsTr("Tilt Angle:") }
        RealValueLabel { value: penInfo.tiltAngle }

        DescriptionLabel { text: qsTr("Tilt Rotation:") }
        RealValueLabel { value: penInfo.tiltRotation }

        PenTiltView {
            tiltRotation: penInfo.tiltRotation
            tiltAngle: penInfo.tiltAngle

            GridLayout.columnSpan: 2

            Layout.fillWidth: true
            Layout.maximumWidth: 200
            Layout.maximumHeight: 200
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: width
        }
    }
}
