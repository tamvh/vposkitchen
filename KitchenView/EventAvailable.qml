import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Item {
    width: listViewEvents.width
    height: detailRow.height + 40    

    Rectangle
    {
        id: background
        anchors.fill: parent
        color: "white"
        Rectangle {
            height: 1
            anchors.bottom: parent.bottom
            width: parent.width
            color: "#D6D6D6"
        }

        RowLayout {
            id: detailRow
            //anchors.left: parent.left
            anchors.margins: 20
            spacing: 10
            anchors.verticalCenter: parent.verticalCenter

            Label {
                anchors.margins:5
                text: title
            }
        }
    }
}
