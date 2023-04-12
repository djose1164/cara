import QtQuick

Image {
    id: root
    property string path: root.unfilled
    readonly property string unfilled: "qrc:/icons/star.svg"
    readonly property string filled: "qrc:/icons/star_fill.svg"
    required property bool selected
    required property int index

    signal clicked(idx: int)

    source: selected ? filled : unfilled
    sourceSize: "24x24"

    TapHandler {
        onTapped:
            clicked(index);
    }
}
