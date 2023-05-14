import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias canUpdate: updateBtn.visible
    property int inStock

    signal updateClicked

    Row {
        Label {
            text: inStock === 1 ? `¡Solo queda ${inStock}!`
                                : inStock ? `¡Solo quedan ${inStock}!`
                                          : `¡Agotado!`
            visible: inStock <= 6
            font.pointSize: 14
        }

        Button {
            id: updateBtn
            text: "Actualizar"
            onClicked: updateClicked()
        }
    }
}
