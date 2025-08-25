pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Templates as T
import Cara.Customer
import Cara.Controls
import Cara.Utils

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    implicitWidth: listView.implicitWidth
    implicitHeight: listView.implicitHeight

    header: ToolBar {
        id: toolBar
        RowLayout {
            anchors.fill: parent

            Label {
                id: label
                horizontalAlignment: Qt.AlignHCenter
                text: "Cuentas Pendientes"
                font.pointSize: 16
                font.bold: true
                Layout.fillWidth: true
            }

            SearchButton {
                id: searchButton
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    root.state = "searching"
                }
            }

            SearchItem {
                id: searchItem
                visible: false
                Layout.fillWidth: true
                Layout.fillHeight: true
                onSearchClosed: root.state = ""
                background.color: Material.primary
            }
        }
    }

    states: [
        State {
            name: "searching"
            when: searchItem.visible
            PropertyChanges {
                label.visible: false
                searchButton.visible: false
                searchItem.visible: true
            }
        }
    ]

    Component.onCompleted: {
        customerWithPendingPayModel.fetchCustomersWithOutstandingOrders()
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: outstandingCusProxyModel
        spacing: 20
        delegate: OutstandingOrder {
            required customerName
            required numberOfOrders
            required totalToPay
            required forename
            required surname
            required customerId

            width: listView.width
        }
    }

    CustomerOutstandingOrderModel {
        id: customerWithPendingPayModel
        customerRepository: RestCustomerRepository {}
    }

    OutstandingCustomersProxyModel {
        id: outstandingCusProxyModel
        sourceModel: customerWithPendingPayModel
        customerName: searchItem.displayText
    }

    component OutstandingOrder: ItemDelegate {
        id: outstandingOrder
        property string customerName
        property int numberOfOrders
        property int totalToPay
        property string forename
        property string surname
        property int customerId

        RectangularShadow {
            anchors.fill: myRectangle
            offset.x: -10
            offset.y: -5
            radius: myRectangle.radius
            blur: 30
            spread: 10
            color: Qt.darker(myRectangle.color, 1.6)
            z: -10
        }

        contentItem: Rectangle {
            id: myRectangle

            readonly property int margin: 15

            implicitWidth: columnLayout.implicitWidth
            implicitHeight: columnLayout.implicitHeight + columnLayout.spacing + myRectangle.margin
            radius: 10

            ColumnLayout {
                id: columnLayout
                anchors.fill: parent
                anchors.margins: myRectangle.margin
                spacing: 5

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: outstandingOrder.customerName
                    font.bold: true
                    font.pointSize: 14
                }

                Label {
                    text: `Total de ordenes por pagar ${outstandingOrder.numberOfOrders}.`
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    horizontalAlignment: Label.AlignRight
                    text: `Total a pagar RD$${outstandingOrder.totalToPay}`
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }

                Item {}
            }
        }

        onClicked: {
            const customerPage = "qrc:/qt/qml/Cara/Customer/CustomerPage.qml"
            const json = {
                "customerId": outstandingOrder.customerId,
                "model": {
                    "forename": outstandingOrder.forename,
                    "surname": outstandingOrder.surname
                }
            }
            root.stackView.pushItem(customerPage, json)
        }
    }
}
