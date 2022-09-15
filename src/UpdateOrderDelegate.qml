import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: stackLayout.width
    height: stackLayout.height

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
        anchors.top: lbl.bottom
        anchors.topMargin: 20
        spacing: 20

        Repeater {
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
