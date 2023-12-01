import QtQuick
import QtQuick.Controls
import Salesperson
import User
import "qrc:/utils"
import "qrc:/singleton"

Page {
    id: root

    required property int salespersonId

    readonly property int fontSize: 12
    readonly property StackView stackView: StackView.view

    title: "Clientes"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }
    Component.onCompleted: spService.fetchCustomers()

    ListView {
        id: listView
        anchors.fill: parent
        model: listModel
        delegate: ItemDelegate {
            id: control

            required property int customer_id
            required property string name

            onClicked: {
                customerEdit.customerId = customer_id;
                customerEdit.open();
            }

            width: listView.width
            text: `${name}\nClient ID: ${customer_id}`
            font.pointSize: 14
        }
    }

    ListModel { id: listModel }

    Label {
        width: listView.width * .9
        anchors.centerIn: parent
        visible: listView.count === 0
        text: "La lista de clientes esta vacia. Inteta agregar uno nuevo."
        font.pointSize: 14
        wrapMode: Label.Wrap
    }

    RoundButton {
        onClicked: stackView.push("qrc:/customers/CustomerForm.qml")

        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 15
        }

        icon.source: "qrc:/icons/person_add.svg"
        icon.width: 30
        icon.height: 30
    }

    AssociateSalespersonService {
        id: spService
        salespersonId: root.salespersonId
        onFetched: listModel.append(JSON.parse(data).customers)
    }

    CustomerEdit {
        id: customerEdit
        width: root.width
        height: root.height
        stackView: root.stackView
    }
}
