import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import Billing
import "qrc:/singleton"

Item {
    id: root

    property StackView stackView
    property TabBar bar
    readonly property bool isUser: CurrentUser.user.userType === UserEntity.USER

    width: 400
    height: 400

    Label {
        id: recentOrdersLbl
        anchors.top: parent.top
        text: "Compras realizadas recientemente "
        font.pointSize: 13

        IconImage {
            anchors.left: recentOrdersLbl.right
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/icons/arrow_forward.svg"
            sourceSize: "18x18"
        }

        TapHandler { onTapped: bar.currentIndex = 2 }
    }

    StackLayout {
        id: recentordersSL
        width: parent.width
        height: parent.height * .2
        anchors.top: recentOrdersLbl.bottom
        anchors.topMargin: 20
        onCurrentIndexChanged: print("recentordersSL: "+currentIndex)
        currentIndex: isUser ? 0 : 2

        BusyIndicator {}

        ListView {
            id: recentOrders
            readonly property StackView stackView: root.stackView
            model: billModel
            delegate: BillDelegate {
                required details
                required payment
                required date
                required orderId

                orientation: ListView.Horizontal
            }
            orientation: ListView.Horizontal
        }

        Item {
            Label {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                text: TheCurrentUser.isGuest ?
                          "Necesitas registrarte para poder ver tus ordenes recientes."
                        : "Aun no haz realizado ninguna orden."
                font.pointSize: 14
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.WordWrap
            }
        }
    }

    BillModel { id: billModel }

    BillBusiness {
        id: billBusiness
        onCompleted: populate()
        onPopulated: recentordersSL.currentIndex = 1
        model: billModel
    }

    Connections {
        target: LoginManager

        function onLoggingOut() {
            recentordersSL.currentIndex = 2;
            billModel.clear();
        }

        function onLoggingIn() {
            recentOrders.currentIndex = 0;
        }

        function onLoggedIn() {
            billBusiness.populate();
        }
    }
}
