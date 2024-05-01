import QtQuick
import QtQuick.Layouts

Item {
    id: root

    readonly property alias searchText: headerItem.displayText
    property alias currentView: stackLayout.currentIndex
    readonly property alias loader: loader

    enum View {
        TitleView,
        SearchView
    }

    implicitWidth: stackLayout.implicitWidth
    implicitHeight: 50

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        Loader {
            id: loader
        }

        SearchItem {
            id: headerItem
            onSearchClosed: stackLayout.currentIndex = SearchHeader.TitleView
        }
    }
}
