import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.qmlmodels
import com.cara 1.0
import "qrc:/utils"
import "qrc:/orders"

ListView {
    id: root

    property bool shouldRefresh
    property StackView stackView
    property alias billBusiness: billBusiness

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
            clearModel();

            populateModel();
        }
        refreshIcon.visible = false;
        shouldRefresh = false;
    }

    delegate: OrderState {
        required details
        required orderId
        required date
        required payment
        required status

        Component.onCompleted: print("orderId: " + orderId)

        width: root.width
    }

    model: billModel

    RefreshIndicator {
        id: refreshIcon
    }

    BusyIndicator {
        anchors.centerIn: parent
        visible: count === 0
    }

    BillModel {
        id: billModel
    }

    BillBusiness {
        id: billBusiness
        onPopulated: modelPopulated()
        model: billModel
    }
}
