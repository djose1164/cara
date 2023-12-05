import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"
import "qrc:/utils"
import "qrc:/customers"
import core
import User
import Salesperson

ListView {
    id: root

    readonly property LocalUserStore userStore: LocalUserStore {}

    delegate: BillDelegate {
        required name
        required property int customer_id
        userType: userStore.user.userType

        customerId: customer_id
    }
    model: CustomerManager.model

    Label {
        id: label
        anchors.centerIn: parent
        visible: count === 0
        width: root.width*.9
        text: "No hay ningun cliente para mostrar."
        font.pointSize: 16
        horizontalAlignment: Label.AlignHCenter
        wrapMode: Label.WordWrap
    }
}
