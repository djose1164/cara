import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Item {
    id: root

    readonly property StackView stackView: StackView.view
    Component.onCompleted: {
        providerApi.fetchProviders();
    }

    Rectangle {
        id: header
        width: root.width
        height: 40
        z: 100
        anchors {
            top: root.top
        }

        color: "floralwhite"

        BackButton {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            stackView: root.stackView
        }

        Label {
            anchors.centerIn: parent
            text: "Proveedores"
            font.pointSize: 16
        }
    }

    ListView {
        id: listView
        width: root.width
        anchors {
            top: header.bottom
            bottom: root.bottom
        }
        spacing: 40
        model: listModel
        delegate: SwipeDelegate {
            id: swipeDelegate
            width: listView.width
            height: 40
            text: name

            ListView.onRemove: removingAnimation.start()

            SequentialAnimation {
                id: removingAnimation
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: true
                }
                NumberAnimation {
                    target: swipeDelegate
                    property: "height"
                    to: 0
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: swipeDelegate
                    property: "ListView.delayRemove"
                    value: false
                }
            }

            swipe.right: Label {
                id: deleteLabel
                text: qsTr("Delete")
                color: "white"
                verticalAlignment: Label.AlignVCenter
                padding: 12
                height: parent.height
                anchors.right: parent.right

                SwipeDelegate.onClicked: listModel.remove(index)

                background: Rectangle {
                    color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
                }
            }
        }

        Label {
            anchors.centerIn: parent
            width: root.width/2
            visible: listModel.count === 0
            text: "Tu lista de proveedores esta vacia. Intenta agregar uno."
            wrapMode: Label.Wrap
        }
    }

    RoundButton {
        id: addBtn
        onClicked: {
            newProviderForm.open();
        }

        anchors {
            right: root.right
            rightMargin: 10
            bottom: root.bottom
            bottomMargin: 20
        }
        text: "+"
        font.pointSize: 14
    }

    ProviderForm {
        id: newProviderForm
        width: root.width * .9
        height: root.height * .8
    }

    ListModel { id: listModel }

    ProviderAPI {
        id: providerApi

        onProvidersFetched: providers => listModel.append(providers)
    }
}
