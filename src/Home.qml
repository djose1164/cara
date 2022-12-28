import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QZXing

Page {
    Item {
        width: parent.width * .9
        height:  parent.height / 2
        anchors.horizontalCenter: parent.horizontalCenter

        GridLayout {
            id: gridLayout
            columns: 2
            width: parent.width
            height: width

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

            Rectangle {
                color: "chocolate"
                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/2
                Button {
                    anchors.centerIn: parent
                    background: Image {
                        anchors.centerIn: parent
                        source: "qrc:/icons/new_product.svg"
                        sourceSize: "48x48"
                    }
                    onClicked: mainStackView.push("qrc:/product/ProductForm.qml", {"pageHeaderView": mainStackView});
                }
            }
            Label {
                text: "The date is: " + new Date().toLocaleDateString(Qt.locale(), "dd/MM/yyyy")
            }
        }
    }
}
