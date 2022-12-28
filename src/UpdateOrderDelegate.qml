import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    width: parent.width
    required property var orders
    required property string forename
    required property string surname
    required property string id

    ColumnLayout {
        Layout.preferredWidth: parent.width * .9
        Layout.alignment: Qt.AlignHCenter
        spacing: 20

        Label {
            id: lbl
            text: "Propietario de orden: " + forename + surname + " ID #" + id
            font.pointSize: 14
        }

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
