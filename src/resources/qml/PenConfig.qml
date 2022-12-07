import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: control

    property var currentConfig: configModel.config(configView.currentIndex)

    component DescriptionLabel: Label {
        font.weight: Font.Light
    }

    component ValueEdit: TextField {
        Layout.fillWidth: true
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: qsTr("Add")
                Layout.fillWidth: true
                onClicked: {
                    configModel.addConfig()
                    configView.currentIndex = configModel.size - 1
                }
            }

            Button {
                text: qsTr("Remove")
                Layout.fillWidth: true
                onClicked: {
                    const index = configView.currentIndex
                    if (index < 0)
                        return

                    configView.currentIndex = -1
                    configModel.removeConfig(index)
                    configView.currentIndex = Math.min(index, configModel.size - 1)
                }
            }
        }

        ListView {
            id: configView
            model: configModel
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            keyNavigationEnabled: true
            delegate: ItemDelegate {
                text: name
                width: ListView.view.width
                highlighted: ListView.isCurrentItem
                onClicked: ListView.view.currentIndex = index
            }
        }

        ColumnLayout {
            enabled: currentConfig
            Layout.fillWidth: true

            DescriptionLabel { text: qsTr("Name:") }
            ValueEdit {
                text: currentConfig ? currentConfig.name : ""
                onTextChanged: if (currentConfig) currentConfig.name = text
            }

            DescriptionLabel { text: qsTr("Presure Levels:") }
            ValueEdit {
                text: currentConfig ? currentConfig.pressureLevels : ""
                onTextChanged: if (currentConfig) currentConfig.pressureLevels = parseInt(text, 10)
            }

            DescriptionLabel { text: qsTr("Tilt (degrees):") }
            ValueEdit {
                text: currentConfig ? currentConfig.tilt : ""
                onTextChanged: if (currentConfig) currentConfig.tilt = parseFloat(text)
            }

            DescriptionLabel { text: qsTr("Resolution (LPI):") }
            ValueEdit {
                text: currentConfig ? currentConfig.resolution : ""
                onTextChanged: if (currentConfig) currentConfig.resolution = parseInt(text, 10)
            }

            DescriptionLabel { text: qsTr("Work Width (inches):") }
            ValueEdit {
                text: currentConfig ? currentConfig.workWidth : ""
                onTextChanged: if (currentConfig) currentConfig.workWidth = parseFloat(text)
            }

            DescriptionLabel { text: qsTr("Work Height (inches):") }
            ValueEdit {
                text: currentConfig ? currentConfig.workHeight : ""
                onTextChanged: if (currentConfig) currentConfig.workHeight = parseFloat(text)
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}