import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// TODO: Display user info and later make it a page. Or start it right now in
// another qml.

Page {
    id: home
    property int counter: 0

    header: Label {
        id: homeLbl
        text: "Inicio"
        font.pointSize: 20
        font.bold: true
    }


    GridLayout {
        width: parent.width

        Label {
            id: lbl
            Layout.alignment: Qt.AlignCenter
            text: "Informacion Personal"
        }
    }

    footer: ToolBar {
        //height: parent.height
        //anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: "blue"
        }
    }
}
