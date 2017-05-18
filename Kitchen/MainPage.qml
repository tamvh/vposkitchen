import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Material.impl 2.0
import QtQuick.Controls.Styles 1.4

import com.vng.gbc.smr 1.0

Page {
    anchors.fill: parent
    property int globalPadding: 10
    property int globalSpacing: 1
    property string _datetime: qsTr("")
    property string _roomid: qsTr("")
    property string url: qsTr("")
    property string starttime: qsTr("")
    property string endtime: qsTr("")
    property int roomId: 0
    property string currentNumber: qsTr("----")

    Connections {
        target: MainController
        onOrderNumber: {
            console.log("onOrderNumber: ", JSON.stringify(newData));
            eventModel.clear();
            eventModel.append(newData);
            if(newData.length > 0) {
                currentNumber = newData[0].number;
                eventModel.remove(newData[0]);
            }
        }
        onStatusBarCode: {
            console.log("barcode status: ", stt);
            if(!stt) {
                sttbarcodedialog.open();
            } else {
                sttbarcodedialog.close();
            }
        }
    }

    Timer {
        id: clock
        interval: 60000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            updateTime();
        }

        function updateTime() {
            var now = new Date();
            txtFullDate.text = now.toLocaleDateString(Qt.locale("vi_VN"), "dddd, d Tháng M, yyyy")
        }
    }

    Timer {
        property bool userInteractive: false
        interval: 60000
        running: true
        repeat: true        
    }

    // Main column
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Title section
        Image {
            id: titleRow
            Layout.preferredHeight: 70
            Layout.fillWidth: true

            source: "qrc:/images/background/topbar.png"

            RowLayout {
                spacing: 0
                anchors.fill: parent
                RowLayout {
                    Layout.leftMargin: 30
                    Layout.fillWidth: true
                    spacing: 20
                    Label {
                        font.family: awesome.family
                        font.pointSize: 20
                        color: "#B1B1B1"
                        renderType: Text.NativeRendering
                        text: awesome.loaded ? awesome.icons.fa_calendar : ""
                        Button {
                                id: secret
                                property bool allow: false
                                opacity: 0.0
                                onClicked: {
                                    allow = true
                                }
                            }
                    }

                    Label {
                        id: txtFullDate
                        Layout.fillWidth: true
                        font.family: montserrat.name
                        color: "#B1B1B1"
                        font.pointSize: 20
                        text: "No data"

                        Button {
                            opacity: 0.0
                            onClicked: {
                                if (secret.allow)
                                    stackView.push(settingPage)
                                secret.allow = false
                            }
                        }
                    }                    
                }
            }
        }

        // Content section
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0
            // Current meeting
            CurrentEvent {
                id: currentEvent
                Layout.fillHeight:  true
                Layout.preferredWidth: parent.width*2/5
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                Rectangle {
                    id: rectContent
                    anchors.fill: parent
                    color: "#BDBDBD"
                    anchors {
                        bottomMargin: 5
                        topMargin: 5
                        rightMargin: 5
                    }

                    ListModel {
                        id: appModel
                        ListElement { number: "0001";}
                        ListElement { number: "0002";}
                        ListElement { number: "0003";}
                        ListElement { number: "0004";}
                        ListElement { number: "0005";}
                        ListElement { number: "0006";}
                        ListElement { number: "0007";}
                    }

                    ListModel {
                        id: eventModel
                    }
                    GridView {
                        anchors.fill: parent
                        cellWidth: rectContent.width/2;
                        cellHeight: rectContent.height/4;
                        focus: true
                        model: eventModel
                        delegate: Item {
                            width: rectContent.width/2;
                            height: rectContent.height/4
                            Rectangle {
                                width: rectContent.width/2;
                                height: rectContent.height/4-globalSpacing;
                                Image {
                                    anchors.fill: parent
                                    fillMode: Image.PreserveAspectCrop
                                    source: "qrc:/images/background/bgright.png"
                                    Label {
                                        id: content1
                                        anchors.centerIn: parent;
                                        text: number;
                                        font.bold: true;
                                        color: "#616161"
                                        font.pixelSize: 165
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        MainController.initialize();
    }

    Component.onDestruction: {
        MainController.uninitialize();
    }

    Popup {
        id: sttbarcodedialog
        modal: true
        focus: true
        width: 200
        height: 200
        x: (stackView.width - sttbarcodedialog.width) / 2
        y: Math.abs(stackView.height -  sttbarcodedialog.height)/2
        Label {
            text: awesome.icons.fa_warning + " " + qsTr("Máy quét Barcode chưa sẵn sàng")
            font.pixelSize: 40
        }
    }
}
