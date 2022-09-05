import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/main"

// TODO: Each page's header should a component with back arrow. Or create it to
// set it for the components that need it.

Page {
    id: home
    property string currentUser

    header: HomeHeader {}

    GridLayout {
        id: gridLayout
        width: parent.width * .9
        height: parent.height * .9
        anchors.centerIn: parent
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

    ListModel {id: userModel}

    footer: ToolBar {
        background: Rectangle {
            anchors.fill: parent
            color: "floralwhite"
        }

        RowLayout {
            anchors.fill: parent
            anchors.bottomMargin: 5
            spacing: 30

            ToolButton {
                id: homeBtn
                Layout.alignment: Qt.AlignCenter
                background: Image {
                    source: "qrc:/icons/home.svg"
                    antialiasing: false
                    sourceSize: iconSize
                }
            }

            ToolButton {
                id: userBtn
                Layout.alignment: Qt.AlignCenter
                background: Image {
                    source: "qrc:/icons/search.svg"
                    antialiasing: false
                    sourceSize: iconSize
                }
                onClicked: mainStackView.push("qrc:/search/SearchPage.qml")
            }

            ToolButton {
                id: shopBtn
                Layout.alignment: Qt.AlignCenter
                background: Image {
                    source: "qrc:/icons/shopping-cart.svg"
                    antialiasing: false
                    sourceSize: iconSize
                }
                onClicked: mainStackView.push("qrc:/orders/OrderHomePage.qml")
            }

            ToolButton {
                id: billBtn
                Layout.alignment: Qt.AlignCenter
                background: Image {
                    source: "qrc:/icons/bill.svg"
                    antialiasing: false
                    sourceSize: "32x32"
                }
                onClicked: mainStackView.push("qrc:/bills/BillPage.qml")
            }
        }
    }

    SidePane {id: sideMenu}
}
