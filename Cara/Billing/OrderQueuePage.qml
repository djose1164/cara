import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Billing
import Cara.Controls
import Cara.Utils

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    required property int salespersonId

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
                    onSearchClosed: stackLayout.currentIndex = OrderQueuePage.TitleView
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
                onClicked: stackLayout.currentIndex = OrderQueuePage.SearchView
            }

            FilterMenu {}
        }
    }

    component FilterMenu : MoreVertButton {
        onClicked: menu.open()

        Menu {
            id: menu
            y: parent.height
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
        height: root.height * .77
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

        ScrollView {
            anchors.fill: parent
            contentWidth: availableWidth

            ButtonGroup {
                buttons: column.children
                onClicked: btn => internal.queueStatus = queueModel.statusStringToId(btn.text)
            }

            ColumnLayout {
                id: column
                width: filterDialog.width

                RadioButton {
                    text: "Sin asignar"
                }

                RadioButton {
                    text: "Asignadas"
                }

                RadioButton {
                    text: "Completadas"
                }

                RadioButton {
                    text: "Canceladas"
                }

                RadioButton {
                    text: "Devueltas"
                }

                RadioButton {
                    text: "Reembolsadas"
                }

                RadioButton {
                    text: "Todas"
                    checked: true
                }
            }
        }
    }

    QueueProxyModel {
        id: queueProxy
        sourceModel: queueModel
        searchString: searchItem.displayText
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        delegate: QueueDelegate {
            height: 250
            width: listView.width
            onClicked: stackView.pushItem("qrc:/qt/qml/Cara/Billing/BillDetailsPage.qml", {
                                              model, mode: BillDetailsPage.QueueMode, proxyModel: queueProxy
                                          });
        }
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
        onOrdersNotFound: internal.busy = false
        onQueueFetched: internal.busy = false
        onErrorOccurred: err => {
            internal.busy = false;
            messagePane.text = err;
            messagePane.visible = true;
        }
    }

    MessagePane {
        id: messagePane
        width: root.width * .9
        height: root.height/2
        anchors.centerIn: parent
        type: MessagePane.Error
        visible: false
    }

    OrderQueueModel {
        id: queueModel
        repository: billBusiness
    }

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
        billBusiness.fetchOrdersInQueue(root.salespersonId);
    }

    function fetchCustomerBills(customerId: int) {
        setFetchingState();
        billBusiness.fetchBillsByCustomerId(customerId);
    }
}
