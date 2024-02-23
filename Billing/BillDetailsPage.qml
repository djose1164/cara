import QtQuick
import QtQuick.Controls
import Utils
import Billing
import "qrc:/utils"
import "qrc:/orders"

BackPage {
    id: root

    required property var model
    property int orderId
    property var date
    property var details
    property var payment
    property var status
    property alias customer: orderState.buyer

    title: "Detalles de orden"
    btnLoader.sourceComponent: menuBtn

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        OrderState {
            id: orderState
            width: {
                if (root.width <= 400)
                    return root.width * .9;
                else if (root.width <= 500)
                    return root.width * .85
                else if (root.width <= 600)
                    return root.width * .8
                else
                    return root.width *.75
            }
            anchors.horizontalCenter: parent.horizontalCenter
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
            model: root.model
        }
    }

    ProcessingIndicator {
        id: processingIndicator
        width: root.width * .8
    }

    Component {
        id: menuBtn
        MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu

                MenuItem {
                    text: "Exportar como PDF"
                    onTriggered: {
                        processingIndicator.open();
                        orderResource.getOrderById(root.orderId);
                    }
                }
            }
        }
    }

    RestService {
        OrderResource {
            id: orderResource
            path: "orders"
            onFetched: resp => {
                orderTemplate.order = JSON.parse(resp).order;
                printer.print();
            }
        }
    }

    PdfOrderPrinter {
        id: printer
        document: OrderTemplate {
            id: orderTemplate
        }

        onOrderPrinted: url => {
            processingIndicator.close();
            Qt.openUrlExternally(url);
        }
    }
}
