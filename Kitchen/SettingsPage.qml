import QtQuick 2.0

import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0

import com.vng.gbc.smr 1.0

Page {
    header: ToolBar {
        Material.foreground: "white"
        Material.primary: "#f78c37"

        RowLayout {
            spacing: 20
            anchors.fill: parent
            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: stackView.depth > 1 ? "images/back.png" : "images/drawer.png"
                }
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                    }
                }
            }

            Label {
                id: titleLabel
                text: "Settings"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsPopup.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                    MenuItem {
                        text: "Quit"
                        onTriggered: Qt.quit()
                    }
                }
            }
        }
    }

    Pane {
        id: settingsPane
        property bool changed: false

            anchors.fill: parent
            ColumnLayout {
            id: settingsColumn
            width: parent.width/2
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            RowLayout {
                spacing: 10
                Label {
                    id: lblRestart
                    text: "Restart required"
                    color: "#e41e25"
                    visible: settingsPane.changed === true ? true : false
                    Layout.fillWidth: true
                }
            }

            // Row setting server url
            RowLayout {
                spacing: 10
                Label {
                    text: "Server:"
                }

                TextField {
                    id: txtServerUrl
                    Layout.fillWidth: true;
                    Component.onCompleted: {
                        text = AppSetting.serverUrl()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }                
            }

            // Row setting update VID
            RowLayout {
                spacing: 10

                Label {
                    text: "VID"
                }

                TextField {
                    id: txtVID
                    Layout.fillWidth: true;

                    Component.onCompleted: {
                        text = AppSetting.getVID()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }
            }

            // Row setting update PID
            RowLayout {
                spacing: 10

                Label {
                    text: "PID"
                }

                TextField {
                    id: txtPID
                    Layout.fillWidth: true;

                    Component.onCompleted: {
                        text = AppSetting.getPID()
                    }

                    onTextChanged: {
                        settingsPane.changed = true;
                    }
                }
            }

            // Row save settings button
            RowLayout {
                spacing: 10

                Button {
                    text: "Save"
                    onClicked:  {
                        if (true === settingsPane.changed) {
                            AppSetting.setServerUrl(txtServerUrl.text);
                            AppSetting.setVID(parseInt(txtVID.text));
                            AppSetting.setPID(parseInt(txtPID.text));
                            AppSetting.save();
                        }
                    }
                }
            }
        }
    }
}
