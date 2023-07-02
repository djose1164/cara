import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

StackLayout {
    id: root

    property alias model: billView.model
    property StackView stackView
    readonly property UserEntity user: CurrentUser.user

    anchors.fill: parent

    onCurrentIndexChanged: {
        if (currentIndex === 1)
            billView.populateModel();
        else if (currentIndex === 2)
            customerView.populateModel();
    }

    BillMessage {}

    BillView {
        id: billView
        stackView: root.stackView
    }

    CustomersView {
        id: customerView
        stackView: root.stackView
    }

    Connections {
        target: LoginManager

        function onLoggingOut() {
            print("---Bills: loggingOut");
            if (CurrentUser.isAdmin)
                customerView.clearModel();
            else
                billView.clearModel();
        }

    }

    function gotoBillMessage() {
        currentIndex = 0;
    }

    function gotoBillsView() {
        currentIndex = 1;
    }

    function gotoCustomersView() {

        currentIndex = 2;
    }
}
