import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GridLayout {
    id: orderHeader

    property var provider
    property var contact: provider.contact
    property string description
    property string date

    columns: 2

    GroupBox {
        Layout.rowSpan: 2
        Layout.alignment: Qt.AlignTop
        Layout.fillHeight: true
        title: "Proveedor"

        ColumnLayout {
            anchors.fill: parent

            Label {
                text: `Nombre: ${provider.name}`
            }

            Label {
                text: `ID: ${provider.id}`
            }

            Label {
                text: `Email: ${contact.email}`
            }

            Label {
                text: `Telefono: ${contact.telephone}`
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: "Descripcion"

        Label {
            width: parent.width
            text: `${description}`
            wrapMode: Label.Wrap
        }
    }

    GroupBox {
        title: "Fecha"
        Label {
            text: `${date}`
        }
    }
}
