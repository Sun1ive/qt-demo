import QtQuick 2.13
import QtQuick.Controls 2.13

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    Rectangle {
        width: parent.width / 2
        height: parent.height / 2
        color: "lightgreen"
    }

    Rectangle {
        anchors.fill: parent
        color: "lightblue"
    }

    Button {
        text: "Download";
        anchors.centerIn: root

        onClicked: function() {
            back.getApproximateTimeWait()
        }
    }
}
