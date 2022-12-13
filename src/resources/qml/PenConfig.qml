import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.settings

Pane {
    id: control

    property var currentConfig: null
    property string currentConfigName: ""
    property string currentConfigPressureLevels: ""
    property string currentConfigTilt: ""

    function updateCurrentConfigInfo() {
        const size = configModel.size
        const index = configView.currentIndex
        const index_valid = index >= 0 && index < size

        configView.currentIndex = index
        currentConfig = index_valid ? configModel.at(index) : null
        currentConfigName = currentConfig ? currentConfig.name : ""
        currentConfigPressureLevels = currentConfig ? currentConfig.pressureLevels : ""
        currentConfigTilt = currentConfig ? currentConfig.tilt : ""
    }

    component DescriptionLabel: Label {
        font.weight: Font.Light
    }

    component ValueEdit: TextField {
        Layout.fillWidth: true
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: configView

            model: configModel
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            keyNavigationEnabled: true

            delegate: ItemDelegate {
                text: config.name
                width: ListView.view.width
                highlighted: ListView.isCurrentItem
                onClicked: ListView.view.currentIndex = index
            }

            onCurrentIndexChanged: updateCurrentConfigInfo()
        }

        ColumnLayout {
            enabled: currentConfig
            Layout.fillWidth: true

            DescriptionLabel { text: qsTr("Name:") }
            ValueEdit {
                text: currentConfigName
                onTextChanged: if (currentConfig) currentConfig.name = text
            }

            DescriptionLabel { text: qsTr("Presure Levels:") }
            ValueEdit {
                text: currentConfigPressureLevels
                onTextChanged: if (currentConfig) currentConfig.pressureLevels = parseInt(text, 10)
            }

            DescriptionLabel { text: qsTr("Tilt (degrees):") }
            ValueEdit {
                text: currentConfigTilt
                onTextChanged: if (currentConfig) currentConfig.tilt = parseFloat(text)
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: addConfigAction
        function onTriggered() {
            configModel.appendConfig()
            configView.currentIndex = configModel.size - 1
        }
    }

    Connections {
        target: removeConfigAction
        function onTriggered() {
            const index = configView.currentIndex
            if (index < 0)
                return

            configView.currentIndex = -1
            configModel.removeConfig(index)
            configView.currentIndex = Math.min(index, configModel.size - 1)
            updateCurrentConfigInfo()
        }
    }

    Connections {
        target: configModel
        function onSizeChanged() {
            updateCurrentConfigInfo()
        }
    }

    Settings {
        category: "PenConfig"
        property alias selectedConfigIndex: configView.currentIndex
    }
}
