import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property int selectedStars

    width: 24*5
    height: 24

    RowLayout {
        anchors.fill: parent

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
