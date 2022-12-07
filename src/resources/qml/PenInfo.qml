import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: control

    component InfoLabel: Label {
        Layout.alignment: Qt.AlignRight
        font.weight: Font.Light
    }

    component ValueLabel: Label {
        font.weight: Font.Medium
        Layout.fillWidth: true
    }

    GridLayout {
        width: parent.width

        columns: 2

        InfoLabel { text: qsTr("Device ID:") }
        ValueLabel { text: penInfo.deviceId }

        InfoLabel { text: qsTr("Device Type:") }
        ValueLabel { text: penInfo.deviceTypeName }

        InfoLabel { text: qsTr("Pointer Type:") }
        ValueLabel { text: penInfo.pointerTypeName }

        InfoLabel { text: qsTr("Pressure:") }
        ValueLabel { text: penInfo.pressure }

        InfoLabel { text: qsTr("Rotation:") }
        ValueLabel { text: penInfo.rotation }

        InfoLabel { text: qsTr("Tangential Pressure:") }
        ValueLabel { text: penInfo.tangentialPressure }

        InfoLabel { text: qsTr("X Tilt:") }
        ValueLabel { text: penInfo.xTilt }

        InfoLabel { text: qsTr("Y Tilt:") }
        ValueLabel { text: penInfo.yTilt }

        InfoLabel { text: qsTr("Z:") }
        ValueLabel { text: penInfo.z }
    }
}
