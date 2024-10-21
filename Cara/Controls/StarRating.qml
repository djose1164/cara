import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property int selectedStars
    property size starSize: "24x24"
    readonly property alias starsCount: internal.starsCount

    signal starClicked(index: int)

    implicitWidth: 24*5
    implicitHeight: 24
    onSelectedStarsChanged: {
        internal.starsCount = root.selectedStars;
        console.log("selectedStarsChanged: "+root.selectedStars);
        //root.updateStarSelection();
    }

    RowLayout {
        anchors.fill: parent

        Repeater {
            model: 5//starModel
            delegate: Star {
                required property int index

                sourceSize: root.starSize
                rating: index
                on: internal.starsCount >= index
                onClicked: {
                    internal.starsCount = rating;

                    //internal.starsCount = newIdx;
                    //root.updateStarSelection();
                    root.starClicked(index);
                }
            }
        }
    }

    QtObject {
        id: internal
        property int starsCount: -1
    }
}
