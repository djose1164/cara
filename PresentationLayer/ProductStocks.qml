import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias canUpdate: updateBtn.visible
    property int inStock

    signal updateClicked

    height: 50
    width: 200

    Row {
        spacing: 20

        Label {
            text: inStock === 1 ? `¡Solo queda ${inStock}!`
                                : inStock ? `¡Solo quedan ${inStock}!`
                                          : `¡Agotado!`
            visible: inStock <= 6
            font.pointSize: 14
        }

        Button {
            id: updateBtn
            text: "Actualizar stocks"
            onClicked: updateClicked()
        }
    }
}
