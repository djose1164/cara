import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

BusyStackLayout {
    id: root

    property alias model: billView.model
    property StackView stackView
    readonly property bool isAdmin: CurrentUser.userType === CurrentUser.ADMIN
    readonly property bool isGuest: CurrentUser.userType === CurrentUser.GUEST

    anchors.fill: parent
    currentIndex: {
        if (isAdmin) {
            return 0;
        } else {
            return isGuest ? 1 : 0
        }
    }

    BillMessage {}

    BillView {
        id: billView
        onModelPopulated: gotoBillsView();
        onFlickEnded: root.currentIndex = 0;
        stackView: root.stackView
    }

    Customers {
        id: customerView
        onModelPopulated: gotoCustomersView();
        stackView: root.stackView
    }

    Connections {
        target: CurrentUser

        function onLoggingOut() {
            print("---Bills: loggingOut");
            if (isAdmin)
                customerView.clearModel();
            else
                billView.clearModel();
        }

        function onLoggedOut() {
            gotoBillMessage();
        }

        function onLoggedIn() {
            print("---Bills: onLoggedIn: userType: "+CurrentUser.userType);
            switch (CurrentUser.userType) {
            case CurrentUser.ADMIN:
                customerView.populateModel();
                break;
            case CurrentUser.USER:
                if (CurrentUser.isCustomer)
                    billView.populateModel();
                else
                    gotoBillMessage();
                break;
            case CurrentUser.GUEST:
                gotoBillMessage();
                break;
            }
        }
    }

    function gotoBillMessage() {
        currentIndex = 1;
    }

    function gotoBillsView() {
        currentIndex = 2;
    }

    function gotoCustomersView() {
        currentIndex = 3;
    }
}
