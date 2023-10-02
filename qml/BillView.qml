import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.qmlmodels
import core 1.0
import "qrc:/utils"

/**
  * This component shows all the bills from the current customer.
*/

ListView {
    id: root

    property bool shouldRefresh
    property StackView stackView

    signal modelPopulated

    delegate: BillDelegate {
        required orderId
        required date
        required details
        required payment
        required status
    }

    model: billModel
    Component.onCompleted: populateModel()

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
