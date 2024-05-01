import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Utils

Item {
    id: root

    property int minCharactersToSearch: 3
    property alias placeholderText: textField.placeholderText
    property alias text: textField.text
    readonly property alias displayText: textField.displayText

    signal accepted(string text)
    signal textEdited

    implicitHeight: textField.implicitHeight

    TextField {
        id: textField
        anchors.fill: parent
        verticalAlignment: TextField.AlignVCenter
        rightPadding: lupa.width + font.pixelSize
        focus: root.visible
        Keys.onEnterPressed: shouldAccept()
        Keys.onReturnPressed: shouldAccept()
        EnterKey.type: Qt.EnterKeySearch
        onTextEdited: root.textEdited()
    }

    IconImage {
        id: lupa
        anchors.right: textField.right
        anchors.rightMargin: 10
        anchors.verticalCenter: textField.verticalCenter
        source: "qrc:/icons/search.svg"

        sourceSize: "16x16"

        TapHandler {
            onTapped: shouldAccept()
        }

        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
    }

    Rectangle {
        anchors.fill: parent
        z: -1
        radius: 5
    }

    function shouldAccept() {
        const text = textField.text.trim();
        if (text.length >= minCharactersToSearch)
            accepted(text);
    }
}
