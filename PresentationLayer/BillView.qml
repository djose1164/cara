import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.cara 1.0

ListView {
    id: root

    property bool shouldRefresh

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
            baseModel.clearModel();
            stackLayout.currentIndex = 0;
            updateCustomerModel();
        }
        refreshIcon.visible = false;
        shouldRefresh = false;
    }

    delegate: Loader {
        required property var model

        source: switch (CurrentUser.userType) {
            case UserEntity.ADMIN:
                return "qrc:/bills/BillDelegate.qml";
            case UserEntity.USER:
                return "qrc:/bills/CustomerBill.qml";
        }
    }

    model: billModel

    Pane {
        id: refreshIcon
        width: parent.width
        height: 40
        visible: false

        Image {
            anchors.centerIn: parent
            source: "qrc:/icons/refresh_black_24dp.svg"

        }
    }

    BillModel {
        id: billModel
    }
}
