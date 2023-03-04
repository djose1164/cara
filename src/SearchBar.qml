import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

PageHeader {
    id: root
    property alias placeholderText: textField.placeholderText
    readonly property string toFind: textField.text
    property bool suggetions: true
    // Set minimun number of characters that have to be present on the textfield
    // to emit accepted signal.
    property int minCharactersToSearch: 3

    signal accepted(string text)

    RowLayout {
        anchors.fill: parent
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.bottom: parent.bottom


        TextField {
            id: textField
            Layout.fillWidth: true
            Layout.leftMargin: 10
            focus: true
            verticalAlignment: TextField.AlignVCenter
            leftPadding: lupa.width + font.pixelSize
            color: "black"
            Keys.onEnterPressed: shouldAccept()
            Keys.onReturnPressed: shouldAccept()

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
        }

        ToolButton {
            background: IconImage {
                source: "qrc:/icons/settings.svg"
                sourceSize: iconSize
            }
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
        }
    }
    function shouldAccept() {
        if (textField.text.length >= minCharactersToSearch)
            accepted(textField.text);
    }
}
