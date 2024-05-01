import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Utils
import Cara.Billing

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    enum View {
        TitleView,
        SearchView
    }

    title: "Cola"
    header: StackLayout {
        id: stackLayout

        TitleHeader {
            id: titleHeader
            title: root.title
            btnLoader.sourceComponent: menuBtn
            onBackClicked: root.stackView.popCurrentItem()
        }

        Rectangle {
            color: Material.backgroundColor

            RowLayout {
                anchors.fill: parent

                SearchItem {
                    id: searchItem
                    Layout.fillWidth: true
                    onSearchClosed: stackLayout.currentIndex = OrderQueue.TitleView
                }

                FilterMenu {
                    Layout.alignment: Qt.AlignRight
                }
            }
        }
    }

    states: [
        State {
            name: "busy"
            when: internal.busy
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        },
        State {
            name: "empty"
            when: !internal.busy && listView.count === 0
            PropertyChanges {
                target: messageLbl
                text: "No hay ninguna orden para mostrar."
            }
        }
    ]

    Component.onCompleted: {
        fetchOrdersInQueue();
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    Label {
        id: messageLbl
        width: root.width * .8
        visible: false
        wrapMode: Label.Wrap
        horizontalAlignment: Label.AlignHCenter
        font.pointSize: 16
    }

    Component {
        id: menuBtn
        RowLayout {
            SearchButton {
                onClicked: stackLayout.currentIndex = OrderQueue.SearchView
            }

            FilterMenu {}
        }
    }

    component FilterMenu : MoreVertButton {
        onClicked: menu.open()

        Menu {
            id: menu
            rightMargin: 5

            MenuItem {
                text: "Filtro"
                onTriggered:filterDialog.open()
            }
        }
    }

    Dialog {
        id: filterDialog
        width: root.width * .8
        height: Math.max(root.height/2, implicitHeight)
        anchors.centerIn: Overlay.overlay
        title: "Filtro"
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            print("Queue: "+internal.queueStatus);
            queueProxy.status = internal.queueStatus;
        }

        ButtonGroup {
            buttons: column.children
        }

        Column {
            id: column
            RadioButton {
                text: "Sin asignar"
                onClicked: internal.queueStatus = OrderQueueEnum.NoAssignment
            }

            RadioButton {
                text: "Asignadas a mi"
                onClicked: internal.queueStatus = OrderQueueEnum.AssingedToMe
            }

            RadioButton {
                text: "Completadas"
                onClicked: internal.queueStatus = OrderQueueEnum.Done
            }

            RadioButton {
                text: "Todas"
                checked: true
                onClicked: internal.queueStatus = OrderQueueEnum.Any
            }
        }
    }

    QueueProxyModel {
        id: queueProxy
        sourceModel: billModel
        searchString: searchItem.displayText
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        delegate: billDelegate
        spacing: 20
        model: queueProxy
    }

    Component {
        id: billDelegate
        CustomerBillDelegate {
            id: customerBillDelegate
            required property var model
            required details
            required payment
            required status
            required customer

            width: root.width
            height: 110
            orderId: model.orderId
            orderDate: model.date
            onClicked: {
                stackView.pushItem("qrc:/qt/qml/Cara/Billing/BillDetailsPage.qml", {
                                   model, mode: BillDetailsPage.QueueMode, proxyModel: queueProxy
                               });
            }
        }
    }

    BillBusiness {
        id: billBusiness
        model: billModel
        onOrdersNotFound: internal.busy = false
        onPopulated: internal.busy = false
    }

    BillModel { id: billModel }

    QtObject {
        id: internal
        property int queueStatus
        property bool busy
    }

    function setFetchingState() {

    }

    function fetchOrdersInQueue() {
        internal.busy = true;
        setFetchingState();
        billBusiness.fetchOrdersInQueue();
    }

    function fetchCustomerBills(customerId: int) {
        setFetchingState();
        billBusiness.fetchBillsByCustomerId(customerId);
    }
}
