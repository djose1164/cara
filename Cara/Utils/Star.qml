import QtQuick

Item {
    id: root

    readonly property string unfilled: "star-unfilled.png"
    property bool on
    property int rating
    property alias sourceSize: image.sourceSize

    signal clicked

    implicitWidth: image.implicitWidth
    implicitHeight: image.implicitHeight

    Image {
        id: image
        source: root.on ? "star.png" : unfilled
    }

    TapHandler {
        onTapped: clicked()
    }
}
