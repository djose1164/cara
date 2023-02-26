import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Order

Home {
    Item {
        width: parent.width * .9
       height:  parent.height *.8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.centerIn: parent

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
                    onClicked: stackView.push("qrc:/provider/ProviderForm.qml");
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
                    onClicked: stackView.push("qrc:/product/ProductForm.qml", {"pageHeaderView": mainStackView});
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
                        source: "qrc:/icons/person_add.svg"
                        sourceSize: "48x48"
                    }
                    onClicked: stackView.push("qrc:/customers/CustomerForm.qml");
                }
            }
        }
    }
}
