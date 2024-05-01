import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    title: "Credito"

    ColumnLayout {
        anchors.fill: parent
        spacing: 15
        enabled: false

        TextField {
            placeholderText: "Disponible"
            text: SalespersonSettings.creditAvailable
            Layout.fillWidth: true
        }

        TextField {
            placeholderText: "Consumido"
            text: SalespersonSettings.creditConsumed
            Layout.fillWidth: true
        }

        TextField {
            placeholderText: "Limite"
            text: SalespersonSettings.creditLimit
            Layout.fillWidth: true
        }
    }
}
