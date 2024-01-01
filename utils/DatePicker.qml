import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root

    property date selectedDate

    anchors.centerIn: Overlay.overlay
    locale: Qt.locale()
    title: "Selecciona una fecha"
    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: root.selectedDate = internal.selectedDate
    Component.onCompleted:  {
        root.selectedDate = internal.currentDate;
        internal.currentTime = internal.currentDate.getTime()
        internal.selectedMonth = calendarModel.indexOf(internal.currentDate);
        //listView.currentIndex = internal.selectedMonth;
        print("calenarmodel.count: "+calendarModel.count);
        print(internal.currentTime);
    } 

    ColumnLayout {
        anchors.fill: parent
        Label {
            id: selectedDateLbl
            text: new Date(internal.currentTime).toLocaleDateString()
            font.pointSize: 12
            Layout.fillWidth: true
        }

        Label {
            id: visibleDate
            Layout.fillWidth: true
            Button {
                property int count
                anchors.right: parent.right
                text: "<"
                font.pointSize: 12
                onClicked: calendarModel.to = new Date(calendarModel.to.getFullYear() - 1, 11, 31)
            }
        }

        ListView {
            id: listView

            property int selectedIndex

            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            snapMode: ListView.SnapOneItem
            spacing: 20
            orientation: ListView.Horizontal
            highlightRangeMode: ListView.StrictlyEnforceRange
            model: calendarModel
            delegate: DateDelegate {
                id: dateDelegate
                height: listView.height
                width: listView.width
                onSelectedDateFromCalendarChanged: internal.selectedDate = selectedDateFromCalendar
            }
        }
    }

    CalendarModel {
        id: calendarModel
        from: new Date(2020, 0, 1)
        to: new Date(2024, 12, 31)
    }

    QtObject {
        id: internal
        readonly property date currentDate: new Date()
        property date selectedDate
        property int selectedMonth: -1
        property double currentTime: -1
        property int lastIndex

        onSelectedDateChanged: print("internal: "+selectedDate)
        Component.onCompleted: selectedMonth = currentDate.getMonth()

        function getTime(wrongDate: date): double {
            wrongDate.setHours(wrongDate.getHours() + 4);
            return wrongDate.getTime();
        }
    }
}
