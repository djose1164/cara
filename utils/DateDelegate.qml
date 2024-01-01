import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    required property var model
    required property int index
    property date selectedDateFromCalendar

    Label {
        id: selectedDate
        text: Qt.locale().monthName(grid.month) + ` de ${grid.year}`
        font.pointSize: 15
    }

    DayOfWeekRow {
        Layout.fillWidth: true
        locale: Qt.locale()
    }

    MonthGrid {
        id: grid
        Layout.fillWidth: true
        Layout.fillHeight: true
        month: model.month
        year: model.year
        locale: Qt.locale()
        delegate: dayDelegate
        onClicked: selectedDate => {
            const currentTime = internal.getTime(selectedDate);
            internal.currentTime = currentTime;
        }
    }

    Component {
        id: dayDelegate
        Label {
            required property int day
            required property int month
            required property int index
            required property var model
            readonly property bool selectedDate: month === grid.month
            readonly property bool selectedTime: internal.currentTime === thisTime
            readonly property double thisTime: internal.getTime(model.date)

            opacity: selectedDate ? 1 : .2
            color: selectedTime ? "white" : "black"
            text: day
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignVCenter
            // onThisTimeChanged: if (selectedTime) root.selectedDateFromCalendar = new Date(thisTime)
            Rectangle {
                width: Math.min(parent.width, parent.height) + 3
                height: width
                anchors.centerIn: parent
                z: -2
                color: selectedTime ? Material.primary : "white"
                radius: 180
                onColorChanged: if(color === Material.primary) root.selectedDateFromCalendar = new Date(thisTime)
            }
        }
    }
    Component.onCompleted: internal.lastIndex = index
}
