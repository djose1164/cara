import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Page {
    header: PageHeader { title: "Ordenes" }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 20

        Rectangle {
            Layout.preferredHeight: parent.height * .4
            Layout.fillWidth: true
            color: "chocolate"

            Label {
                anchors.centerIn: parent
                text: "Agregar orden"
            }

            TapHandler {
                onTapped: mainStackView.push("qrc:/orders/AddOrderPage.qml")
            }
        }

        Rectangle {
            Layout.preferredHeight: parent.height * .4
            Layout.fillWidth: true
            color: "chocolate"

            Label {
                anchors.centerIn: parent
                text: "Actualizar orden"
            }

            TapHandler {
                onTapped: mainStackView.push("qrc:/orders/UpdateOrderPage.qml")
            }
        }
    }
}
