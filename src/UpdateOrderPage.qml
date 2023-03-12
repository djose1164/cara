import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils
import "qrc:/customers" as Client

// TODO: Update the view after effecting a payment.

Page {
    id: root

    required property var person_info
    required property ListModel orders
    readonly property StackView myStack: StackView.view
    readonly property string name: `${person_info.forename} ${person_info.surname}`

    signal paymentApplied

    header: Utils.TitleHeader {
        title: "Cliente: "+name+" ID #"+person_info.customer_id;
        label.font.pointSize: 12
        stackView: myStack
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 20
        model: orders
        delegate: OrderState {
            Component.onCompleted: paymentApplied.connect(root.paymentApplied)
        }
    }
}
