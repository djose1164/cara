import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: root
    required property StackView stackView

    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    signal acceptedText(text: string)

    RowLayout {
        anchors.fill: parent
        spacing: 20

        ToolButton {
            id: settingBtn
            background: Image {
                source: "qrc:/icons/menu-burger.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: sideMenu.open()
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 5
            Layout.leftMargin: 5
        }

        Label {
            id: homeLbl
            text: "Inicio"
            color: "black"
            font.pointSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
        }

        TabButton {
            id: userBtn
            background: IconImage {
                source: "qrc:/icons/search.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: {
                stackView.push("qrc:/search/SearchPage.qml")
                const item = stackView.currentItem;
                // BUG: SearchBar is not working. I don't know even if the model
                // is being binded to the SearchPage.
                item.acceptedText.connect(function(t) {
                    root.acceptedText(t);
                });
            }
        }
    }
}
