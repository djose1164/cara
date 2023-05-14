import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Order

Home {
    GridLayout {
        id: gridLayout
        width: parent.width*.9
        height: parent.height*.7
        columns: 2
        anchors.centerIn: parent
        columnSpacing: 15
        rowSpacing: 15

        Repeater {
            model: model
            delegate: iconOption
        }
    }

    Component {
        id: iconOption
        Rectangle {
            id: rec
            required property string source
            required property string path
            color: "chocolate"
            Layout.fillWidth: true
            Layout.fillHeight: true
            states: [
                State {
                    name: "hovered"
                    when: control.hovered
                    PropertyChanges {
                        rec {
                            scale: 1.15
                            color: "crimson"
                        }
                    }
                }
            ]
            transitions: [
                Transition {
                    PropertyAnimation {
                        property: "scale"
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }

                    ColorAnimation { duration: 200 }
                }
            ]

            Image {
                anchors.centerIn: parent
                source: rec.source
                sourceSize: "48x48"
            }

            TapHandler { onTapped: stackView.push(path) }

            HoverHandler { id: control }
        }
    }

    ListModel {
        id: model

        ListElement {
            source: "qrc:/icons/complete_order.svg"
            path: "qrc:/provider/ProviderForm.qml"
        }

        ListElement {
            source: "qrc:/icons/new_product.svg"
            path: "qrc:/product/ProductForm.qml"
        }

        ListElement {
            source: "qrc:/icons/person_add.svg"
            path: "qrc:/customers/CustomerForm.qml"
        }

        ListElement {
            source: "qrc:/icons/inventory.svg"
            path: "qrc:/inventory/Inventory.qml"
        }
    }
}
