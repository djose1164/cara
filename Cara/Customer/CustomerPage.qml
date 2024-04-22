import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara
import Cara.Utils
import Cara.Customer
import Cara.Billing

BackPage {
    id: root

    required property int customerId
    property var model

    title: "Cliente"

    Component.onCompleted: {
        customerResource.readSummary(customerId);
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        GridLayout {
            columns: 2
            rows: 2
            columnSpacing: 25
            rowSpacing: 10

            Image {
                id: customerPic
                source: "person.svg"
                sourceSize: "48x48"
                Layout.rowSpan: 2
            }

            RichLabel {
                id: name
                text: `Nombre: ${internal.template.arg(model.forename)}`
            }

            RichLabel {
                id: lastName
                text: `Apellido: ${internal.template.arg(model.surname)}`
            }
        }

        RedirectLabel {
            title: "Ver informacion personal"
            Layout.fillWidth: true
            onClicked: {
                customerEdit.customerId = customerId;
                customerEdit.open();
            }
        }

        RedirectLabel {
            title: "Ver todas las ordenes"
            Layout.fillWidth: true
            onClicked: {
                stackView.pushItem("qrc:/qt/qml/Cara/Billing/BillHomePage.qml", {customerId});
            }
        }

        GroupBox {
            title: "Resumen de Cuentas"
            font.pointSize: 12
            Layout.fillWidth: true

            ColumnLayout {
                id: billsResumeColumn
                anchors.fill: parent

                MarkdownLabel {
                    visible: listModel.count === 0
                    text: "No se ha encontrado ninguna orden."
                    horizontalAlignment: Label.AlignHCenter
                    padding: 14
                    font.bold: true
                    Layout.fillWidth: true
                }
                Repeater {
                    model: listModel
                    delegate: SummaryDelegate {}

                    // MarkdownLabel {
                    //     text: `Ultima compra: `
                    // }
                }
            }
        }
    }

    CustomerEdit {
        id: customerEdit
        width: root.width
        height: root.height
    }

    RestService {
        CustomerResource {
            id: customerResource
            path: "customers"
            onFetched: data => {
                const summary = JSON.parse(data).summary;
                if (summary.length > 0)
                    listModel.append(summary);
            }
        }
    }

    ListModel {
        id: listModel
    }

    QtObject {
        id: internal
        readonly property int fontSize: 5
        readonly property string template: `<b><font size="${internal.fontSize}">%1</font></b>`
    }

    component MarkdownLabel: Label {
        textFormat: Label.MarkdownText
    }

    component RichLabel: Label {
        textFormat: Label.RichText
    }

    component SummaryDelegate: Column {
        id: summaryDelegate
        required property int payment_status_id
        required property int total
        required property int index

        width: billsResumeColumn.width

        MarkdownLabel {
            text: `${statusIdToString(payment_status_id)}: **${total}**`
            padding: 14

            function statusIdToString(statusId: int): string {
                if (statusId === Payment.Paid)
                    return "Pagadas";
                else if (statusId === Payment.Unpaid)
                    return "A credito";
                else
                    return "Abonadas";
            }
        }

        MenuSeparator {
            width: billsResumeColumn.width
            visible: index !== listModel.count - 1
        }
    }
}
