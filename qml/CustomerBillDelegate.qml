import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    id: root

    property int orderId
    property date date
    readonly property string delegateUrl: "qrc:/orders/OrderState.qml"

    ColumnLayout {
        anchors.fill: parent

        Label {
            Layout.leftMargin: 5
            text: `Orden #${root.orderId}`
            font.pointSize: 16
        }
        Label {
            Layout.leftMargin: 15
            Layout.fillWidth: true
            text: `**Fecha:** ${root.date.toLocaleDateString()}`
            textFormat: Label.MarkdownText
            wrapMode: Label.Wrap
        }
    }
}
