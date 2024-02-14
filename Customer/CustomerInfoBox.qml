import QtQuick
import QtQuick.Controls
import Customer

Item {
    id: root

    property customer customer

    implicitHeight: groupBox.implicitHeight
    GroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Informacion del Cliente"
        font.pointSize: 12

        Label {
            anchors.fill: parent
            text: `
            Nombre completo: <h3>${customer.forename} ${customer.surname}</h3>
            ID: <h3>${customer.customerId}</h3>
            `
        }
    }
}
