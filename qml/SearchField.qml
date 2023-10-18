import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

TextField {
    id: textField

    property int minCharactersToSearch: 3

    signal accepted(string text)

    verticalAlignment: TextField.AlignVCenter
    leftPadding: lupa.width + font.pixelSize
    color: "black"
    Keys.onEnterPressed: shouldAccept()
    Keys.onReturnPressed: shouldAccept()
    EnterKey.type: Qt.EnterKeySearch

    ToolButton {
        id: lupa
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        background: Image {
            antialiasing: false
            source: "qrc:/icons/search.svg"
            sourceSize: "18x18"
        }
        onClicked: shouldAccept()
    }

    function shouldAccept() {
        const text = textField.text.trim();
        if (text.length >= minCharactersToSearch)
            accepted(text);
    }
}
