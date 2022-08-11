import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    ToolButton {
        background: Image {
            source: "qrc:/icons/left_arrow.svg"
            sourceSize: iconSize
        }
        onClicked: mainStackView.pop()
        Layout.topMargin: 5
    }

    Label {
        text: "Facturas"
        font.bold: true
        font.pointSize: 18
        Layout.alignment: Qt.AlignCenter
        Layout.topMargin: 5
    }

    ToolButton {
        background: Image {
            source: "qrc:/icons/settings.svg"
            sourceSize: iconSize
        }
        Layout.alignment: Qt.AlignRight
        Layout.topMargin: 5
        Layout.rightMargin: 5
    }
}
