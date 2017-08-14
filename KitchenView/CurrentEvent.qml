import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "controls" as Awesome

Item {
    id: root
    clip: true;
    property bool available: true
    property variant event: undefined
    property int marginGlobal: 10


    Rectangle {
        anchors.fill: parent
        anchors.margins: 5
        //color: "#F77711"
        Image {
            id: name
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/images/background/bgleft.png"
        }
    }

    Item {
        anchors.fill: parent
        Item {
            anchors.fill: parent
            Label {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: mainPage.currentNumber
                font.family: montserrat.name
                font.pixelSize: 250
                font.weight: Font.Medium
                color: "white"
            }
        }
    }
}
