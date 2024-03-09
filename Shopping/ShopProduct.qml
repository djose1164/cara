import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property list<string> locations
    property int sellPrice
    property int stock

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        PriceSpinBox {
            enabled: internal.canBuy
            to: internal.topLimit
            unitPrice: root.sellPrice
            Layout.fillWidth: true
        }

        Button {
            text: internal.canBuy ? "Agregar al Carrito" : "Mas uninades en camino"
            enabled: internal.canBuy
            Layout.fillWidth: true
        }

        Label {
            text: "Cantidad disponible: " + root.stock
        }

        Label {
            id: availableStockLocation
            text: root.getAvailableLocations()
            wrapMode: Label.Wrap
            font.pointSize: 12
            Layout.fillWidth: true
        }
    }

    QtObject {
        id: internal
        readonly property bool canBuy: root.stock > 0
        readonly property int topLimit: canBuy ? root.stock : 1
    }

    function getAvailableLocations(): string {
        const template = "Disponible en: %1";

        const length = root.locations.length;
        if (length === 0)
            return template.arg("-");

        let finalLocations = "";
        for (let i = 0; i < length; ++i) {
            const location = root.locations[i];
            if ((i + 1) === length)
                finalLocations += `${location}`;
            else
                finalLocations += `${location}, `;
        }
        print(template.arg(finalLocations))
        return template.arg(finalLocations);
    }
}
