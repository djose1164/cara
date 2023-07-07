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

    Component.onCompleted: billBusiness.fetchBillsByCustomerId(customerId)

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
        id: busyIndicator
        anchors.centerIn: parent
        visible: count === 0
    }

    Label {
        id: label
        visible: false
        anchors.centerIn: parent
        width: root.width*.9
        text: "Este cliente no cuenta con ninguna orden."
        font.pointSize: 14
        wrapMode: Label.WordWrap
        horizontalAlignment: Label.AlignHCenter
    }

    BillModel {
        id: billModel
    }

    BillBusiness {
        id: billBusiness
        onPopulated: {
            if (billModel.count === 0) {
                busyIndicator.visible = false;
                label.visible = true;
            }

            modelPopulated();
        }
        model: billModel
    }
}
