import QtQuick
import Cara.Controls

Item {
    id: root

    property string title: "Titulo"
    readonly property alias searchText: searchHeader.searchText

    signal closeClicked

    implicitWidth: searchHeader.implicitWidth
    implicitHeight: searchHeader.implicitHeight

    SearchHeader {
        id: searchHeader
        anchors.fill: parent
        loader.sourceComponent: CloseHeader {
            title: root.title
            loader.sourceComponent: SearchButton {
                onClicked: searchHeader.currentView = SearchHeader.SearchView
            }
            onCloseClicked: root.closeClicked()
        }
    }
}
