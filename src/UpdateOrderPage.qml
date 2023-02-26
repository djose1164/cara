import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/search" as Search
import "qrc:/utils" as Utils
import "qrc:/customers" as Client
import "qrc:/main"

// TODO: Update the view after effecting a payment.

Page {
    id: root

    required property var person_info
    required property ListModel orders
    readonly property StackView myStack: StackView.view
    readonly property string name: `${person_info.forename} ${person_info.surname}`

    header: Utils.PageHeader {
        title: "Cliente: "+name+" ID #"+person_info.customer_id;
        label.font.pointSize: 12
        stackView: myStack
    }

    Component.onCompleted: {
        print(
            "parent's height "  + parent.height + "\n"+ "lbl's height "
            + lbl.height + "\n" +
            "listView's height " + listView.height
        )
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 20
        model: orders
        delegate: OrderDelegate {
            onDataChanged: {
                print("UpdateOrderPage: UpdateOrderDelegate: onDataChanged");
                customerModel.setById(clientId);
            }
        }
    }
}
