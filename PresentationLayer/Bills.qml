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

    anchors.fill: parent
    currentIndex: isAdmin ? 3 : 1

    BillMessage {}

    BillView {
        id: billView
        //onModelPopulated: gotoBillsView();
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
            print("---Bills: onLoggedIn: userType: "+CurrentUser.userType)
            if (isAdmin)
                customerView.populateModel();
        }

        function onUserTypeChanged() {
            print("---Bills: onUserTypeChanged "+CurrentUser.userType)
            switch (CurrentUser.userType) {
            case CurrentUser.ADMIN:

                break;
            case CurrentUser.CUSTOMER:
                // Fixme: Handle USER in its own branch
            case CurrentUser.USER:
                billView.populateModel();
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
