import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/utils"

ListView {
    id: root

    property StackView stackView
    property bool shouldRefresh

    signal modelPopulated

    onContentYChanged: {
        if (contentY < -90) {
            refreshIcon.visible = true;
            shouldRefresh = true;
        }
        //print(`onContentYChanged: Content Y is: ${contentY}`)
    }
    onFlickEnded: {
        print(`onFlickEnded: Content Y is: ${contentY}`)
        if (shouldRefresh) {
            customerModel.clear();

            populateModel();
        }
        refreshIcon.visible = false;
        shouldRefresh = false;
    }

    delegate: BillDelegate {
        required property string fullname
        required customerId

        name: fullname
    }

    model: customerModel

    CustomerModel {
        id: customerModel

        onPopulated: modelPopulated();
    }

    RefreshIndicator {
        id: refreshIcon
    }

    BusyIndicator {
        anchors.centerIn: parent
        visible: root.count === 0
    }

    function populateModel() {
        customerModel.populate();
    }

    function clearModel() {
        customerModel.clear();
    }
}
