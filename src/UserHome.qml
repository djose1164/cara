import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"
import "qrc:/product"
import "qrc:/customers"
import "qrc:/bills"

// TODO: This component it's getting too big! A design by sections sould be make
// to each section would correspond a custom Component.

Home {
    id: root

    readonly property string fullName: TheCurrentUser.isUser ?
                                           ` ${TheCurrentUser.forename} ${TheCurrentUser.surname}`
                                         : ""

    Component.onCompleted: productRequest.addSuggetionsToModel()

    Item {
        id: lbl
        width: parent.width
        height: parent.height * .1
        Label {
            id: label
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: `¡Bienvenido${fullName}, mira lo que tenemos disponible para ti!`
            font.pointSize: 14
            wrapMode: Text.WordWrap
        }

        Label {
            anchors {
                top: label.bottom
                topMargin: 10
                bottom: parent.bottom
                left: parent.left
                leftMargin: 5
            }
            text: "¡Aprovecha las ofertas!"
            font.pointSize: 13
        }
    }

    StackLayout {
        id: offerStackLayout
        width: parent.width
        height: parent.height * .4
        anchors.top: lbl.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter

        BusyIndicator {}

        ListView {
            id: listView
            readonly property StackView stackView: root.stackView

            model: productRequest.model
            delegate: ProductDelegate {}
            orientation: ListView.Horizontal
        }
    }

    ProductRequest {
        id: productRequest
        onSucceeded: offerStackLayout.currentIndex = 1
    }

    Item {
        id: recentOrdersItem
        anchors.top: offerStackLayout.bottom
        anchors.topMargin: 20
        width: root.width
        height: 25
        Label {
            id: recentOrdersLbl
            anchors.top: parent.top
            anchors.verticalCenter: parent.verticalCenter
            text: "Compras realizadas recientemente "
            font.pointSize: 13
        }
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
        anchors.top: recentOrdersItem.bottom
        anchors.topMargin: 20
        onCurrentIndexChanged: print("recentordersSL: "+currentIndex)

        BusyIndicator {}

        ListView {
            id: recentOrders
            readonly property StackView stackView: root.stackView
            model: customerModel.model
            delegate: CustomerBill { orientation: ListView.Horizontal }
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

    CustomerModel {
        id: customerModel
        request.onSucceeded: recentordersSL.currentIndex = 1
    }

    Connections {
        target: TheCurrentUser
        // TODO: Optimize this for later release.
        function onOrderPosted() {
            customerModel.findOrdersByCustomerId(TheCurrentUser.customerId);
        }

        function onUserCredentialsSet() {
            print("function onUserCredentialsSet() isAdmin: " + TheCurrentUser.isAdmin);
           // recentordersSL.currentIndex = 1;
            if (TheCurrentUser.isCostumer)
                customerModel.findOrdersByCustomerId(TheCurrentUser.customerId);
        }

        function onNotLoggedIn() { recentordersSL.currentIndex = 2; }

        function onLoggedOut() { recentordersSL.currentIndex = 2; }
    }
}
