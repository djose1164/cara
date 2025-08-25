import QtQuick
import QtQuick.Layouts
import Cara.Controls

Item {
    id: root

    enum View {
        Empty,
        Selected
    }

    property alias emptyDelegate: emptyLoader.sourceComponent
    property alias selectedDelegate: selectedLoader.sourceComponent
    readonly property alias emptyItem: emptyLoader.item
    readonly property alias selectedItem: selectedLoader.item
    readonly property bool isSelectedView: stackLayout.currentIndex === EntitySelector.Selected

    implicitWidth: stackLayout.implicitWidth
    implicitHeight: !root.isSelectedView ? stackLayout.implicitHeight*.45 : stackLayout.implicitHeight

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: internal.hasSelection ? EntitySelector.Selected
                                            : EntitySelector.Empty

        Loader {
            id: emptyLoader
        }

        Loader {
            id: selectedLoader
        }
    }

    QtObject {
        id: internal
        property bool hasSelection
    }

    function validate() {
        if (!internal.hasSelection)
            throw "Necesitas elegir un cliente";
    }

    function setSelectedView() {
        stackLayout.currentIndex = EntitySelector.Selected;
    }

    function setEmptyView() {
        stackLayout.currentIndex = EntitySelector.Empty;
    }
}
