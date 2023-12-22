import QtQuick
import QtQuick.Controls
import Salesperson
import User
import Utils
import "qrc:/utils"
import "qrc:/singleton"

ListViewPage {
    id: root

    required property int salespersonId

    readonly property int fontSize: 12
    readonly property StackView stackView: StackView.view

    title: "Clientes"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }
    noDataMessage: userStore.user.isAdmin ? "Este vendedor no cuenta con ningun cliente."
                                          : "La lista de clientes esta vacia. Inteta agregar uno nuevo."

    listView {
        model: listModel
        delegate: ItemDelegate {
            id: control

            required property int customer_id
            required property string name

            width: listView.width
            text: `${name}\nClient ID: ${customer_id}`
            font.pointSize: 14
            onClicked: {
                customerEdit.customerId = customer_id;
                customerEdit.open();
            }
        }
    }

    Component.onCompleted: {
        spService.fetchCustomers();
        root.state = "fetching";
    }

    ListModel { id: listModel }

    LocalUserStore { id: userStore }

    RoundButton {
        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 15
        }
        visible: userStore.user.isSalesperson
        icon.source: "qrc:/icons/person_add.svg"
        icon.width: 30
        icon.height: 30
        onClicked: stackView.push("qrc:/customers/CustomerForm.qml")
    }

    AssociateSalespersonService {
        id: spService
        salespersonId: root.salespersonId
        onFetched: {
            listModel.append(JSON.parse(data).customers);
            root.state = "";
        }
    }

    CustomerEdit {
        id: customerEdit
        width: root.width
        height: root.height
        stackView: root.stackView
    }
}
