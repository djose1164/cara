import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import Customer

Rectangle {
    id: root

    property alias currentIndex: comboBox.currentIndex
    readonly property alias currentValue: comboBox.currentValue

    implicitWidth: 100
    implicitHeight: 64

    ComboBox {
        id: comboBox
        width: root.width
        height: root.height*.9
        anchors.verticalCenter: parent.verticalCenter
        popup.height: 350
        textRole: "name"
        valueRole: "customer_id"
        currentIndex: -1
        displayText: currentIndex === -1 ? "Seleccionar Cliente" : currentText
        model: CustomerManager.model
    }
}
