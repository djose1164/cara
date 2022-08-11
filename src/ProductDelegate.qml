import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    width: searchListView.width
    /*Label {
        text: "Código: " + id
    }*/
    Rectangle {
        anchors.fill: parent
        color: "lightgray"
    }

    Label {
        text: "Nombre: " + name
    }

    /*Label {
        text: "Descripción: " + description
    }*/

    Label {
        text: "Precio: " + sell_price
    }
}
