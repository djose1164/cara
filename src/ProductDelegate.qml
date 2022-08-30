import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    width: searchListView.width
    /*Label {
        text: "Código: " + id
    }*/

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
