import QtQuick
import QtQuick.Controls
import Salesperson
import "qrc:/utils"

Page {
    id: root

    required property int adminId
    readonly property StackView stackView: StackView.view

    title: "Vendedor Asociado"
    header: TitleHeader {
        title: root.title
        backBtn.onClicked: stackView.pop()
    }
    Component.onCompleted: if (SalespersonManager.isEmpty) service.readAll()

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 15
        model: SalespersonManager.model
        delegate: salespersonDelegate
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
        onFetched: SalespersonManager.add(JSON.parse(buffer).salespersons)
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
                text: user.contact.forename +" "+ user.contact.surname + `\nID #${id}`
                padding: 12
                font.pointSize: 12
            }
        }
    }
}
