import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GridLayout {
    id: gridLayout
    columns: 2

    Repeater {
        model: 4
        // TODO: Add animation on hover.
        Rectangle {
            color: "chocolate"
            Layout.preferredWidth: parent.width/2
            Layout.preferredHeight: parent.height/2
            Button {
                anchors.centerIn: parent
                background: Image {
                    anchors.centerIn: parent
                    source: "qrc:/icons/complete_order.svg"
                }
                onClicked: mainStackView.push("qrc:/provider/ProviderForm.qml");
            }
        }
    }
}
