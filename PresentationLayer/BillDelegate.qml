/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"
import "qrc:/customers"

ItemDelegate {
    id: root
    property string name
    property int customerId
    property var orders
    property int orientation : ListView.Vertical

    readonly property int widthLV: ListView.view.width
    readonly property int heightLV: ListView.view.height
    readonly property bool isHorizontal: orientation === ListView.Horizontal

    signal updateCustomer(customerId: int)

    width: ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .15

    onClicked: {
        stackView.push("qrc:/orders/UpdateOrderPage.qml", {
                           name,
                           customerId,
                           orders
                       });
    }

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
            text: `Cliente ID # ${customerId}`
            font.pointSize: 12
            Layout.leftMargin: 15
        }
    }
}
