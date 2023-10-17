import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import entities
import core
import "qrc:/singleton"
import "qrc:/utils"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    readonly property UserEntity user: CurrentUser.user
    readonly property string adminViewPath: "qrc:/bills/CustomersView.qml"
    readonly property string customerViewPath: "qrc:/bills/BillView.qml"
    readonly property string messagePath: "qrc:/bills/BillMessage.qml"

    title: "Facturas"
    header: pageHeader

    BillPageHeader {
        id: pageHeader
        stackView: root.stackView
        buttonLoader.sourceComponent: moreVertBtn
        onDoSearch: billStack.push("qrc:/bills/BillSearch.qml", {model: loader.item.model});
    }

    Component {
        id: moreVertBtn

        MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu

                MenuItem {
                    text: "Filtros"
                }

                MenuItem {
                    text: "Cambiar vista"
                    onTriggered: billViewChooser.open()
                }
            }
        }
    }

    BillViewChooser {
        id: billViewChooser
        width: root.width*.8
        height: root.height/2
        onViewChanged: newView => print("Requested view: " + newView)
    }

    Loader {
        id: loader
        property StackView stackView: root.stackView
        readonly property int customerId: user.customerId
        anchors.fill: parent
    }

    Connections {
        target: LoginManager

        function onLoggedOut() {
            CustomerManager.clearModel();
        }
    }

    states: [
        State {
            name: "userNotLoggedIn"
            when: user.userType === UserEntity.GUEST
            PropertyChanges {
                target: loader
                source: messagePath
            }
        },
        State {
            name: "userLoggedIn"
            when: user.userType === UserEntity.USER
            PropertyChanges {
                target: loader
                source: user.customerId ? customerViewPath : messagePath
            }
        },
        State { // FIXME: This does not makes sense. userLoggedIn is checking this
            name: "adminLoggedIn"
            when: CurrentUser.isAdmin
            PropertyChanges {
                target: loader
                source: adminViewPath
            }
        }
    ]
}
