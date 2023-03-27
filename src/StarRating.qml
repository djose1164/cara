import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property int selectedStars


    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                model: starModel
                delegate: Star { onClicked: idx => starClicked(idx) }
            }
        }
        Label {
            text: `${selectedStars} de ${starModel.count} seleccionadas.`
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

    function starClicked(idx) {
        selectedStars = idx + 1;
        for (let i = 0; i < starModel.count; ++i) {
            const star = starModel.get(i);
            if (i <= idx)
                star.selected = true;
            else
                star.selected = false;
        }
    }

}
