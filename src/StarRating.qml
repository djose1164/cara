import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property int selectedStars

    RowLayout {
        id: starsRowLayout
        anchors.top: parent.top
        anchors.left: parent.left
        width: root.width > 200 ? root.width*.4 : root.width
        height: root.height*.7
        spacing: 10

        Repeater {
            model: starModel
            delegate: Star { onClicked: idx => selectedStars = idx + 1 }
        }
    }

    ListModel {
        id: starModel

        ListElement { selected: false }

        ListElement { selected: false }

        ListElement { selected: false }

        ListElement { selected: false }

        ListElement { selected: false }
    }

    onSelectedStarsChanged: {
        for (let i = 0; i < starModel.count; ++i) {
            const star = starModel.get(i);
            if (i <= selectedStars-1)
                star.selected = true;
            else
                star.selected = false;
        }
    }

}
