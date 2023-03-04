/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    id: root
    required property var person_info
    required property var orders
    required property StackView stackView
    property string name: person_info.forename
                          + (person_info.surname !== null ? ` ${person_info.surname}` : "")

    signal updateCustomer(customerId: int)

    width: ListView.view.width
    height: ListView.view.height * .15

    onClicked: stackView.push("qrc:/orders/UpdateOrderPage.qml", {
                                      "person_info": person_info,
                                      "orders": orders
                                  })

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 15

        Label {
            Layout.topMargin: 15
            text: name
            font.pointSize: 16
        }

        Label {
            text: `Cliente ID # ${person_info.customer_id}`
            font.pointSize: 12
            Layout.leftMargin: 15
        }
    }
}
