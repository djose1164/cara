import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var model
    property int index
    property date selectedDateFromCalendar

    ColumnLayout {
        anchors.fill: parent
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
                internal.selectedtime = currentTime;
                internal.dateWasClicked = true;
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
                readonly property date correctedDate: new Date(internal.getTime(model.date))
                property bool selectedTime: (!internal.dateWasClicked
                                             && internal.currentDate.getDate() === correctedDate.getDate()
                                             && internal.currentDate.getMonth() === correctedDate.getMonth()
                                             && internal.currentDate.getFullYear() === correctedDate.getFullYear())
                                            || internal.selectedtime === internal.getTime(model.date)

                opacity: selectedDate ? 1 : .2
                color: selectedTime ? "white" : "black"
                text: day
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter

                Rectangle {
                    width: Math.min(parent.width, parent.height) + 3
                    height: width
                    anchors.centerIn: parent
                    z: -2
                    color: selectedTime ? Material.primary : "white"
                    radius: 180
                    onColorChanged: if (color === Material.primary) root.selectedDateFromCalendar = new Date(internal.getTime(model.date))
                }
            }
        }
    }

    QtObject {
        id: internal
        property date currentDate: new Date()
        property double selectedtime
        property bool dateWasClicked

        function getTime(wrongDate: date): double {
            wrongDate.setHours(wrongDate.getHours() + 4);
            return wrongDate.getTime();
        }
    }
}
