/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"

ItemDelegate {
    id: root
    required property var order_details
    required property var payment
    required property int id
    required property string date
    property int orientation : ListView.Vertical
    readonly property StackView stackView: ListView.view.stackView

    signal updateCustomer(customerId: int)

    width: ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .15

    onClicked: stackView.push("qrc:/orders/CustomerOrders.qml",
    {
        "date": date, "id": id, "order_details": order_details, "payment": payment
    })

    ColumnLayout {
        anchors.fill: parent
        Label {
            Layout.leftMargin: 5
            text: `Orden #${id}`
            font.pointSize: 16
        }
        Label {
            Layout.leftMargin: 15
            text: `**Fecha:** ${date}`
            textFormat: Label.MarkdownText
        }
    }
}
