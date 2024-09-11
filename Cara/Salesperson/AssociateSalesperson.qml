import QtQuick
import QtQuick.Controls
import Cara.Salesperson
import QtQuick.Templates as T
import Cara.Controls

Page {
    id: root

    required property var user

    readonly property int adminId: user.userId
    readonly property int salespersonId: SalespersonSettings.salespersonId

    readonly property T.StackView stackView: StackView.view

    title: "Vendedor Asociado"
    header: TitleHeader {
        title: root.title
        backBtn.onClicked: stackView.pop()
    }

    states: [
        State {
            name: "fetching"
            PropertyChanges {
                target: busyIndicator
                visible: true
            }
            PropertyChanges {
                target: listView
                visible: false
            }
        }
    ]
    Component.onCompleted: {
        if (SalespersonManager.isEmpty){
            service.readSalespeople();
            root.state = "fetching";
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 15
        model: SalespersonManager.model
        delegate: salespersonDelegate

        Label {
            anchors.centerIn: parent
            width: listView.width * .8
            visible: listView.count === 0
            text: "No cuentas con ningun vendedor asociado. Intenta agregar uno."
            wrapMode: Label.Wrap
            font.pointSize: 14
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    RoundButton {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        text: "+"
        font.pointSize: 16
        font.bold: true
        onClicked: stackView.push("NewSalesperson.qml", {adminId})
    }

    SalespersonController {
        id: service
        adminId: root.adminId
        onFetched: {
            SalespersonManager.add(JSON.parse(data).salespeople);
            root.state = "";
        }
    }

    Component {
        id: salespersonDelegate
        ItemDelegate {
            required property int id
            required property var user
            required property int credit_limit
            required property int credit_available
            required property int credit_consumed

            width: listView.width
            height: label.implicitHeight
            spacing: 5
            onClicked: stackView.push("delegate/AssociateSalespersonDelegate.qml", {
                                          salespersonId: id,
                                          user,
                                          creditLimit: credit_limit,
                                          creditAvailable: credit_available,
                                          creditConsumed: credit_consumed
                                      })

            Label {
                id: label
                text: user.forename +" "+ user.surname + `\nID #${id}`
                padding: 12
                font.pointSize: 12
            }
        }
    }
}
