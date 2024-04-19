import QtQuick
import QtQuick.Controls

Item {
    id: root

    property bool like

    signal clicked(like: bool)

    RoundButton {
        flat: true
        icon.source: root.like ? internal.favoriteFillIcon : internal.favoriteIcon
        onClicked: {
            root.like = !root.like;
            root.clicked(root.like);
        }
    }

    QtObject {
        id: internal
        readonly property url favoriteIcon: "favorite.svg"
        readonly property url favoriteFillIcon: "favorite_fill.svg"
    }
}
