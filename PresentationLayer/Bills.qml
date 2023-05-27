import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

BusyStackLayout {
    id: root

    property alias model: orderView.model

    anchors.fill: parent

    BillMessage {}

    BillView {
        id: orderView
        onModelPopulated: gotoBillsView();
    }

    CustomerView {
        id: customerView
        onModelPopulated: gotoCustomersView();
    }

    Connections {
        target: CurrentUser

        function onUserEntityReady() {
            print("[BillPage] Setting bill view.. ");
            if (CurrentUser.userType === CurrentUser.ADMIN) {
                customerView.populateModel();
                print("[BillPage] Bill admin view.");
            } else {
                orderView.populateModel();
                print("[BillPage] Bill customer view.");
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
