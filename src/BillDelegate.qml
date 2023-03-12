/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"

ItemDelegate {
    id: root
    required property var person_info
    required property ListModel orders
    property int orientation : ListView.Vertical
    readonly property StackView stackView: ListView.view.stackView
    property string name: person_info.forename
                          + (person_info.surname !== null ? ` ${person_info.surname}` : "")

    signal updateCustomer(customerId: int)

    width: ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .15

    onClicked: stackView.push("qrc:/orders/UpdateOrderPage.qml", {
                                  "person_info": person_info,
                                  "orders": orders
                              })

    ColumnLayout {
        anchors.fill: parent
        Layout.leftMargin: 15
        Layout.alignment: Qt.AlignVCenter

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
