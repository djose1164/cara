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
            if (CurrentUser.userType === CurrentUser.ADMIN) {
                customerView.populateModel();
            } else {
                orderView.populateModel();
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
