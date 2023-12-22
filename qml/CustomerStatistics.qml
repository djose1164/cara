import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/product"

Item {
    id: root

    required property int salespersonId
    readonly property StackView stackView: StackView.view
    property string headerTitle

    Component.onCompleted: statisticsApi.fetchBillsStatistics()

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
            width: listView.width
            text: "Estados de Cuentas"
            wrapMode: Label.WordWrap
            padding: 12
            font.pointSize: 14
            font.bold: true

        }

        section {
            property: "status"
            criteria: ViewSection.FullString
            delegate: Label {
                required property string section

                width: listView.width
                background: Rectangle { color: "tomato" }
                color: "white"
                padding: 12
                text: section
                font.pointSize: 12
                font.bold: true
            }
        }
        model: unpaidBills
        delegate: Row {
            Label {
                width: root.width/2
                text: `${name}\nCantidad de cuentas: ${bill_quantity}\n`
                padding: 12
                wrapMode: Label.Wrap
            }

            Label {
                readonly property int toPay: amount_to_pay - paid_amount

                width: root.width/2
                text: status === "Pago" ? `Total pagado: RD$${paid_amount}`
                                        : status === "Credito" ? `Total a pagar: RD$${amount_to_pay}`
                                                               : `Abonado: RD$${paid_amount}\nA pagar: RD$${toPay}`
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
        salespersonId: root.salespersonId
        onCustomerStatisticsFetched: bills => unpaidBills.append(bills)
    }
}
