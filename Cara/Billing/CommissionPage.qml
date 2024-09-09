pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Billing

ListView {
    id: root

    required property CommissionModel commissionModel
    readonly property T.StackView stackView: StackView.view

    header: Header {}
    headerPositioning: ListView.OverlayHeader
    footer: Legend {}
    footerPositioning: ListView.PullBackFooter
    model: root.commissionModel
    delegate: CommissionDelegate {}
    spacing: 5

    Component.onCompleted: {
        console.log("commissionModel.rowCount():",commissionModel.rowCount());
        if (commissionModel.rowCount() === 0)
            commissionModel.fetchCommissions();
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
                onClicked: root.stackView.popCurrentItem()
            }

            Label {
                text: "Historal"
                font.pointSize: 14
                font.bold: true
            }
        }
    }

    component Legend : ToolBar {
        width: root.width
        implicitHeight: rowLayout.implicitHeight
        z: 2

        RowLayout {
            id: rowLayout
            anchors.centerIn: parent

            LegendItem {
                color: "green"
                text: "Pagado"
            }

            LegendItem {
                color: "yellow"
                text: "Sin pagar"
            }
        }
    }

    component LegendItem : Row {
        property alias color: rectangle.color
        property alias text: label.text

        padding: 8
        spacing: 5

        Rectangle {
            id: rectangle
            implicitWidth: 20
            implicitHeight: implicitWidth
            color: "green"
        }

        Label {
            id: label
            text: "Pagado"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    component CommissionDelegate : Item {
        id: commissionDelegate
        required property var model
        required property int salespersonId
        required property int commissionId
        required property int adminId
        required property double amount

        width: ListView.view.width
        height: itemDelegate.implicitHeight

        ItemDelegate {
            id: itemDelegate
            anchors {
                fill: parent
                margins: 5
            }

            text: `
            # de comision: <b>${commissionDelegate.commissionId}</b>
            <br>
            ID de vendedor: <b>${commissionDelegate.salespersonId}</b>
            <br>
            ID de admin: <b>${commissionDelegate.adminId}</b>
            <br>
            Total de ganancias : <b>RD$${commissionDelegate.amount}</b>
            `

            icon {
                source: "commission.png"
                color: "transparent"
                width: 32
                height: width
            }

            background: Rectangle {
                id: rectBg
                readonly property color suggestedColor: isNaN(commissionDelegate
                                                              .model
                                                              .paymentDate)
                                                        ? "yellow" : "green"
                radius: 10
                anchors.margins: 5
                color: itemDelegate.hovered ? Qt.darker(rectBg.suggestedColor,
                                                        1.2)
                                            : rectBg.suggestedColor
            }

            font.pointSize: 14

            onClicked: {
                const repository = root.commissionModel;
                root.stackView.pushItem(Qt.resolvedUrl("CommissionItemsPage.qml"), {model: commissionDelegate.model, repository});
            }
        }
    }
}
