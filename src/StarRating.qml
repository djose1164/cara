import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property int selectedStars

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            Repeater {
                model: starModel
                delegate: Star { onClicked: idx => selectedStars = idx + 1 }
            }
        }
        Label {
            text: `${selectedStars} de ${starModel.count} estrellas seleccionadas.`
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
