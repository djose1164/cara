import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/product"

Item {
    id: root

    readonly property StackView stackView: StackView.view
    property string headerTitle

    Component.onCompleted: statisticsApi.fetchStatistics()

    Header {
        id: header
        title: headerTitle
    }

    ListView {
        id: listView
        anchors.top: header.bottom
        width: root.width
        height: root.height - header.height

        header: Label {
            width: root.width
            text: "Cuentas sin pagar"
            padding: 10
            font.pointSize: 14
            font.bold: true
            wrapMode: Label.Wrap
        }
        model: unpaidBills
        delegate: Row {
            Label {
                width: root.width
                text: `${name}\nCantidad: ${bill_quantity}`
                padding: 12
                wrapMode: Label.Wrap
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            visible: root.count === 0
        }
    }

    ListModel { id: unpaidBills }

    StatisticsAPI {
        id: statisticsApi

        onUnpaidBillsFetched: bills => unpaidBills.append(bills)
    }
}
