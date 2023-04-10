import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    property alias stackLayout: popupStackLayout
    property alias quantity: quantityField.text

    signal okClicked

    width: parent.width*.8
    height: parent.height/2
    anchors.centerIn: Overlay.overlay
    modal: true

    StackLayout {
        id: popupStackLayout
        anchors.fill: parent
        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 20
            property var sendableProduct
            Label {
                Layout.fillWidth: true
                text: `Cantidad disponible en inventario ${inStock}.`
                wrapMode: Label.Wrap
            }

            TextField {
                id: quantityField
                Layout.fillWidth: true
                placeholderText: "Nueva cantidad..."
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 20
                Button {
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    text: "Cancelar"
                    onClicked: updateInStock.close()
                }
                Button {
                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth: true
                    text: "Ok"
                    onClicked: okClicked()
                }
            }
        }
        Label {
            text: "Operacion exitosa"
            font.pointSize: 14
        }

        Label {
            text: "Operacion fallida"
            font.pointSize: 14
        }
    }
}
