import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"

// TODO: This component it's getting too big! A design by sections sould be make
// to each section would correspond a custom Component.

Home {
    id: root

    readonly property string fullName: TheCurrentUser.isUser ?
                                           ` ${TheCurrentUser.forename} ${TheCurrentUser.surname}`
                                         : ""
    readonly property bool isUserAndCustomer: TheCurrentUser.isUser
                                              && TheCurrentUser.isCostumer

    Component.onCompleted: {
        if (isUserAndCustomer)
            setCustomerOrders();
        offers.addSuggetionsToModel();
    }

    Column {
        id: column

        spacing: 20

        Welcome {
            id: welcome
            width: root.width
            fullName: root.fullName
        }

        Offers {
            id: offers
            width: root.width
            height: root.height*.4
            stackView: root.stackView
        }

        RecentOrders {
            id: recentOrders
            width: root.width
            stackView: root.stackView
            bar: root.bar
        }
    }

    Connections {
        target: TheCurrentUser
        // TODO: Optimize this for later release.

        function onUserCredentialsSet() {
            print("function onUserCredentialsSet() isAdmin: " + TheCurrentUser.isAdmin);
           // recentordersSL.currentIndex = 1;
            if (TheCurrentUser.isCostumer)
                setCustomerOrders();
        }
    }

    function setCustomerOrders() {
        recentOrders.setCustomerOrders();
    }
}
