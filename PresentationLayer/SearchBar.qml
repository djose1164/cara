import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

ToolBar {
    id: root
    property alias placeholderText: textField.placeholderText
    readonly property string toFind: textField.text
    property bool suggetions: true
    property StackView stackView

    property int minCharactersToSearch: 3

    signal accepted(string text)

    RowLayout {
        anchors.fill: parent

        BackButton {
            Layout.alignment: Qt.AlignLeading
            stackView: root.stackView
        }

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
        const text = textField.text.trim();
        if (text.length >= minCharactersToSearch)
            accepted(text);
    }
}
