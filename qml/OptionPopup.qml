import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    width: parent.width * .9
    height: parent.height * 1.4
    anchors.centerIn: Overlay.overlay
    modal: true

    property int clientId

    GridLayout {
        anchors.fill: parent
        columns: 2

        Label {
            text: "Que deseas actualizar?"
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignCenter
        }

        Button {
            text: "Perfil"
            Layout.alignment: Qt.AlignCenter
        }

        Button {
            text: "Ordenes"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                billPage.stackView.push("qrc:/orders/UpdateOrderPage.qml",
                                          {"clientId": clientId});
                root.close();
            }
        }
    }
}
