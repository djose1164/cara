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
    Component.onCompleted: {
        root.selectedDate = internal.currentDate;
        internal.selectedMonth = calendarModel.indexOf(internal.currentDate);
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
        }

        ListView {
            id: listView
            clip: true
            spacing: 20
            orientation: ListView.Horizontal
            snapMode: ListView.SnapOneItem
            highlightRangeMode: ListView.StrictlyEnforceRange
            model: calendarModel
            delegate: DateDelegate {
                id: dateDelegate
                required model
                required index

                height: listView.height
                width: listView.width !== 0 ? listView.width : 150
                onSelectedDateFromCalendarChanged: internal.selectedDate = selectedDateFromCalendar
            }
            ScrollIndicator.horizontal: ScrollIndicator {}
            Layout.fillWidth: true
            Layout.fillHeight: true

            onCountChanged: {
                const newIndex = count - 1;
                positionViewAtEnd();
                currentIndex = newIndex;
            }
        }
    }

    CalendarModel {
        id: calendarModel
        from: {
            const tmp = new Date();
            tmp.setFullYear(tmp.getFullYear() - 1)
            return new Date(tmp);
        }
        to: new Date()
    }

    QtObject {
        id: internal

        readonly property date currentDate: new Date()
        property date selectedDate
        property int selectedMonth: -1
        property double currentTime: currentDate.getTime()
        property int lastIndex

        Component.onCompleted: selectedMonth = currentDate.getMonth()

        function getTime(wrongDate: date): double {
            wrongDate.setHours(wrongDate.getHours() + 4);
            return wrongDate.getTime();
        }
    }

    function toShortDate() {
        return selectedDate.toLocaleDateString(root.locale, "yyyy-MM-dd")
    }
}
