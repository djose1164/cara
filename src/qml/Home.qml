import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: home
    property int counter: 0

    header: Label {
        id: homeLbl
        text: "Inicio"
        font.pointSize: 20
        font.bold: true
    }

    Rectangle {
        anchors.fill: parent
        color: "gray"
    }

    RowLayout {
        width: parent.width

        Label {
            id: lbl
            text: helper.getText()
        }


        Button {
            text: "Click me!"
            onClicked: {
                ++counter
                helper.setText("Yay you've clicked me " + counter + " times!")
                lbl.text = helper.getText()
        }}
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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
