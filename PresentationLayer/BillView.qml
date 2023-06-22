import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.qmlmodels
import com.cara 1.0
import "qrc:/utils"

ListView {
    id: root

    property bool shouldRefresh
    property StackView stackView

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

    delegate: BillDelegate {
        required orderId
        required date
        required details
        required payment
        required status
    }

    model: billModel

    RefreshIndicator {
        id: refreshIcon
    }

    BillModel {
        id: billModel
    }

    BillBusiness {
        id: billBusiness
        onPopulated: modelPopulated()
        model: billModel
    }

    function populateModel() {
        billBusiness.populate();
    }

    function clearModel() {
        billBusiness.clearModel();
    }
}
