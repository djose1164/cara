import QtQuick
import QtQuick.Controls
import "qrc:/users"
import "qrc:/product"
import "qrc:/customers"
import "qrc:/bills"

// TODO: This component it's getting too big! A design by sections sould be make
// to each section would correspond a custom Component.

Home {
    id: root

    readonly property string fullName: TheCurrentUser.forename + " "
                                       + TheCurrentUser.surname

    Component.onCompleted: {
        productRequest.addSuggetionsToModel()
        print("From UserHome, adding date to recent orders.");
        customerModel.findOrdersByCustomerId(TheCurrentUser.customerId);
    }
    Connections {
        target: header
        function onAcceptedText(t) {
            productRequest.findProductByIdentifier(t);
        }
    }
    Rectangle {
        id: lbl
        width: parent.width
        height: parent.height * .1
        color: "floralwhite"
        z: 10
        Label {
            id: label
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: `¡Bienvenido ${fullName}, mira lo que tenemos disponible para ti!`
            font.pointSize: 14
            wrapMode: Text.WordWrap
        }

        Label {
            anchors.top: label.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            text: "¡Mira las ofertas que tenemos para ti!"
            font.pointSize: 13
        }
    }

    ListView {
        id: listView
        readonly property StackView stackView: root.stackView
        width: parent.width
        height: parent.height * .4
        anchors.top: lbl.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter

        model: productRequest.model
        delegate: ProductDelegate {}
        orientation: ListView.Horizontal
    }

    ProductRequest { id: productRequest }

    Item {
        id: recentOrdersItem
        anchors.top: listView.bottom
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

    ListView {
        id: recentOrders
        readonly property StackView stackView: root.stackView
        width: parent.width
        height: parent.height * .2
        anchors.top: recentOrdersItem.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        model: customerModel.model
        delegate: CustomerBill {
            width: recentOrders.width/2
            orientation: ListView.Horizontal
        }
        orientation: ListView.Horizontal
    }

    CustomerModel { id: customerModel }

    Connections {
        target: TheCurrentUser
        // TODO: Optimize this for later release.
        function onOrderPosted() {
            customerModel.findOrdersByCustomerId(TheCurrentUser.customerId);
        }
    }
}
