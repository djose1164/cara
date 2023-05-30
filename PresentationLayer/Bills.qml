import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

BusyStackLayout {
    id: root

    property alias model: orderView.model
    property StackView stackView

    anchors.fill: parent
    currentIndex: CurrentUser.userType === CurrentUser.ADMIN? 2 : 1

    BillMessage {}

    BillView {
        id: orderView
        onModelPopulated: gotoBillsView();
        stackView: root.stackView
    }

    Customers {
        id: customerView
        onModelPopulated: gotoCustomersView();
        stackView: root.stackView
    }

    Connections {
        target: CurrentUser

        function onUserEntityReady() {
            switch (CurrentUser.userType) {
            case CurrentUser.ADMIN:
                customerView.populateModel();
                break;
            case CurrentUser.CUSTOMER:
                // Fixme: Handle USER in its own branch
            case CurrentUser.USER:
                orderView.populateModel();
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
