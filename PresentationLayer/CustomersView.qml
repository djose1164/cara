import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"
import "qrc:/utils"

ListView {
    id: root

    property StackView stackView

    delegate: BillDelegate {
        required name
        required property int customer_id

        customerId: customer_id
    }

    model: CustomerManager.model

    BusyIndicator {
        anchors.centerIn: parent
        visible: root.count === 0
    }

    Label {
        id: label
        anchors.centerIn: parent
        visible: false
        width: root.width*.9
        text: "No hay ningun cliente registrado. Agrega uno nuevo."
        font.pointSize: 16
        horizontalAlignment: Label.AlignHCenter
        wrapMode: Label.WordWrap
    }

    Timer {
        interval: 2000
        onTriggered: if (count === 0) label.visible = true
        running: true
    }
}
