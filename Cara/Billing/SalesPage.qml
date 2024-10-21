pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Billing

ListView {
    id: root

    required property SaleRepository repository
    readonly property T.StackView stackView: StackView.view

    implicitWidth: 400
    implicitHeight: 640
    header: Header {}
    headerPositioning: ListView.OverlayHeader
    spacing: 20
    model: saleModel
    delegate: SaleDelegate {
        width: root.width
    }

    Component.onCompleted: {
        saleModel.fetchSales();
        console.log("SalesPage: "+root.width+"x"+root.height);
    }

    component Header: ToolBar {
        width: root.width
        z: 2

        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "←"
                font.pointSize: 14
                font.bold: true
            }

            Label {
                text: "Historal"
                font.pointSize: 14
                font.bold: true
            }
        }
    }

    component SaleDelegate: Item {
        id: saleDelegate

        required property var model
        required property int saleId
        required property int customerId
        required property date createdAt
        required property double totalAmount

        implicitWidth: 100
        implicitHeight: columnLayout.implicitHeight

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            anchors.margins: 15

            Label {
                text: "Sale ID: " + saleDelegate.saleId
            }

            Label {
                text: "Customer ID: " + saleDelegate.customerId
            }

            Label {
                text: "Total Amount: " + saleDelegate.totalAmount
            }

            Label {
                text: saleDelegate.createdAt.toLocaleString()
            }
        }
    }

    SaleModel {
        id: saleModel
        repository: root.repository
    }
}
