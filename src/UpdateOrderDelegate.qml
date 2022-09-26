import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: stackLayout.width
    height: stackLayout.height
    required property var orders
    required property string forename
    required property string surname

    Label {
        id: lbl
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        text: "Propietario de orden " + forename + surname
        font.pointSize: 14
    }

    ColumnLayout {
        width: parent.width
        anchors.top: lbl.bottom
        anchors.topMargin: 20
        spacing: 20

        Repeater {
            Layout.fillWidth: true
            model: orders
            delegate:  OrderDelegate {}
        }
    }

    function statusToString(status) {
        switch(status) {
        case 0:
            return "Pago";
        case 1:
            return "Credito";
        case 2:
            return "Abonado";
        }
    }
}
