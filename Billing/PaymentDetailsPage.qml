import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Utils
import Billing

Page {
    id: root

    required property int paymentId
    readonly property T.StackView stackView: StackView.view

    title: "Detalles de pago"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    Component.onCompleted: paymentResource.refresh()

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        model: paymentResource.data
        delegate: paymentDelegate
        spacing: 20
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
    }

    Component {
        id: paymentDelegate
        Item {
            required property var modelData
            readonly property int amount: modelData.amount
            readonly property date created_at: modelData.created_at
            readonly property string dateString: created_at.toLocaleDateString() + " " + created_at.toTimeString()

            width: listView.width
            height: label.implicitHeight * 1.25

            Frame {
                anchors.fill: parent
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                Label {
                    id: label
                    width: parent.width
                    text: `<span style="font-size: 11pt;">RD$</span><b style="font-size: 15pt;">${amount}</b><h3>${created_at.toLocaleString()}</h3>`
                    textFormat: Label.RichText
                    wrapMode: Label.Wrap
                }
            }
        }
    }

    RestService {
        PaymentResource {
            id: paymentResource
            path: "/payments"
            paymentId: root.paymentId
            onDataChanged: busyIndicator.running = false
        }
    }
}
