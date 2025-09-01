pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Billing
import Cara.Controls

BackPage {
    id: root

    required property var repository
    required property var model

    title: "Detalles"

    component CardBox : GroupBox {
        id: cardBox
        required property string name
        required property int identifier

        Layout.fillWidth: true

        Card {
            implicitHeight: 75
            anchors.fill: parent
            source: "qrc:/qt/qml/Cara/Customer/person.svg"
            text: internal.cardTemplate
                    .arg(cardBox.name)
                    .arg(cardBox.identifier)
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            id: columnLayout
            anchors {
                fill: parent
                margins: scrollView.effectiveScrollBarWidth
            }

            spacing: 30

            CardBox {
                title: "Info del vendedor"
                name: root.model.sellerName
                identifier: root.model.sellerId
            }

            CardBox {
                title: "Info del administrador"
                name: root.model.adminName
                identifier: root.model.adminId
            }

            GroupBox {
                title: "Info de la comisión"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent

                    Label {
                        text: `
                        <h3>ID:</h3>
                        ${internal.fontTag.arg(root.model.commissionId)}
                        <h3>Total ganancia neta:</h3>
                        ${internal.fontTag.arg("RD$" + model.grossProfit)}
                        <h3>% a descontar:</h3>
                        ${internal.fontTag.arg((model.rate * 100) + "%")}
                        <h3>Monto a pagar:</h3>
                        ${internal.fontTag.arg("RD$" + model.amount)}
                        <h3>Fecha de creación:</h3>
                        ${internal.fontTag.arg(model.createdAt.toLocaleString())}
                        `
                        textFormat: Label.RichText
                        wrapMode: Label.Wrap
                        Layout.fillWidth: true
                    }
                }
            }

            GroupBox {
                title: "Info de pago"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    Image {
                        source: internal.isPaymenDateValid ? "paid.png" : "unpaid.png"
                        sourceSize: Qt.size(80, 80)
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Label {
                        text: {
                            if (internal.isPaymenDateValid)
                                return `<h3>Pagado el: ${root.model.paymentDate.toLocaleDateString()}
                                        ${model.paymentDate.toLocaleTimeString(Locale.ShortFormat)}</h3>`;
                            else
                                return "<h3>Aún no se ha efectuado el pago. "
                                        + `El monto a pagar es de RD$${model.amount}</h3>`;
                        }
                        wrapMode: Label.Wrap
                        Layout.fillWidth: true
                    }

                    Button {
                        visible: !internal.isPaymenDateValid
                        text: "Pagar"
                        Material.background: Material.Green
                        Layout.fillWidth: true
                        onClicked: {
                            root.model.paymentDate = new Date();
                        }
                    }
                }
            }

            GroupBox {
                title: `Detalles de la comisión - Total: <b>RD$${root.model.grossProfit}</b>`
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0
                    Repeater {
                        id: repeater
                        model: root.model.commissionItems
                        CommissionItemDelegate {
                            Layout.fillWidth: true
                        }
                    }
                }
            }

            Item {}
        }
    }

    component CommissionItemDelegate : ItemDelegate {
        id: commissionItem
        required property var model

        enabled: false
        icon {
            source: model.imageUrl
            width: 64
            height: width
            color: "transparent"
        }

        text: `<h3>Comisión unitaria: RD$${model.unitCommission}</h3>`
              + `<h3>Cantidad: ${model.quantity}</h3>`
              + `<h3>Total: ${model.quantity * model.unitCommission}</h3>`
    }

    QtObject {
        id: internal
        readonly property string cardTemplate: `<h3>%1</h3><h5>ID #%2</h5>`
        readonly property bool isPaymenDateValid: !isNaN(root.model.paymentDate)
                                                  && root.model.paymentDate <= new Date()
        readonly property string fontTag: `<font size="+1">%1</font>`
    }
}
