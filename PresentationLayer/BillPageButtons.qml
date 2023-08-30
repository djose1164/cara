import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara

RowLayout {
    id: root

    signal openFilter
    signal doSearch

    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
    Layout.rightMargin: 10

    ToolButton {
        visible: CurrentUser.isAdmin
        background: Image {
            antialiasing: false
            source: "qrc:/icons/search.svg"
            sourceSize: iconSize
        }
        Layout.rightMargin: 19
        onClicked: doSearch()
    }

    ToolButton {
        background: Image {
            source: "qrc:/icons/settings.svg"
            sourceSize: iconSize
        }
        onClicked: openFilter()
    }
}

