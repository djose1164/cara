import QtQuick
import QtQuick.Controls
import Cara.Salesperson

Item {
    id: root

    required property int adminId
    readonly property bool selected: comboBox.currentIndex !== -1
    readonly property alias currentValue: comboBox.currentValue

    implicitWidth: comboBox.implicitWidth
    implicitHeight: comboBox.implicitHeight

    Component.onCompleted: {
        spController.readSalespeople();
    }

    ComboBox {
        id: comboBox
        anchors.fill: parent
        textRole: "fullname"
        valueRole: "salespersonId"
        model: salespersonModel
        displayText: currentIndex === -1 ? "Seleccionar vendedor" : currentText
    }

    SalespersonModel {
        id: salespersonModel
    }

    SalespersonController {
        id: spController
        adminId: root.adminId
        onFetched: {
            const salespeople = JSON.parse(data).salespeople;
            salespeople.forEach(sp => salespersonModel.append(spController.fromJsonObject(sp)))
        }
    }
}
