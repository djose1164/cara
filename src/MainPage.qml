import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders
import "qrc:/users"

Page {
    id: root
    readonly property StackView stackView: StackView.view

    //anchors.fill: parent
    footer: MainFooter { id: bar }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex
        /*onCurrentIndexChanged: {
            if (bar.addOrderIdx === currentIndex)
                orderStackView.currentItem.viewRequested();
        }*/

        StackView {
            id: homeView
            initialItem: BusyIndicator {}
        }

        StackView {
            id: orderStackView
            initialItem: BusyIndicator {}
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    Connections {
        target: TheCurrentUser
        function onUserCredentialsSet() {
            print("onUserCredentialsSet: " + TheCurrentUser.homePath)
            homeView.push(TheCurrentUser.homePath)

            print("orderStackView pusing: " + TheCurrentUser.orderFormPath);
            orderStackView.push(TheCurrentUser.orderFormPath);
        }

        function onFailedToSetUserCredentials(response) {
            print("onFailedToSetUserCredentials");
            stackView.replace(errorView, response);
        }
    }

    Component {
        id: errorView
        Item {
            property alias text: label.text
            required property string message
            required property int code
            width: parent.width
            height: parent.height
            Rectangle {
                width: parent.width * .9
                height: parent.height/2
                anchors.centerIn: parent
                border.width: 1
                radius: 10
                color: "floralwhite"
            Label {
                id: label
                width: parent.width * .9
                anchors.centerIn: parent
                anchors.margins: 40
                text: `Error code ${code}: ${message}`
                font.pointSize: 14
                wrapMode: Label.WordWrap
            }
        }
        }
    }

    SidePane { id: sideMenu }
}
