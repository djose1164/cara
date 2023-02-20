import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    width: parent.width
    required property var orders
    required property var person_info
    required property string id
    readonly property string fullname: person_info.forename
                                       + " " + person_info.surname

    signal dataChanged

    ColumnLayout {
        Layout.preferredWidth: parent.width * .9
        Layout.alignment: Qt.AlignHCenter
        spacing: 20

        Label {
            id: lbl
            text: "Propietario de orden: " + fullname + " ID #" + id
            font.pointSize: 14
        }

        Repeater {
            Layout.fillWidth: true
            model: orders
            delegate:
                OrderDelegate {
                onDataChanged: {
                    print("UpdateOrderDelegate: OrderDelegate: onDataChanged");
                    root.dataChanged();
                }
            }
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
