import QtQuick
import QtQuick.Controls

ItemDelegate {
    id: root
    required property string name
    required property int sell_price
    width: ListView.view.width
    height: 75
    anchors.topMargin: 20
    anchors.bottomMargin: 20


    //height: 100
    /*Label {
        text: "Código: " + id
    }*/

    Label {
        id: nameLbl
        text: "Nombre: " + name
    }

    /*Label {
        text: "Descripción: " + description
    }*/

    Label {
        anchors.top: nameLbl.bottom
        anchors.topMargin: 10
        text: "Precio: " + sell_price
    }
}

