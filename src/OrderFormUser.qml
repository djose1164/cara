import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    header: AddOrderHeader {}

    ColumnLayout {
        width: parent.width*.95
        height: parent.height*.95
        anchors.centerIn: parent

        AddOrder {
            id: addOrder
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Label {
            text: `El total asciende a RD${addOrder.totalToPay}`
        }

        Button {
            id: okBtn
            text: "Ok"
            onClicked: addOrder.okHandler()
            Layout.fillWidth: true
        }
    }
}
