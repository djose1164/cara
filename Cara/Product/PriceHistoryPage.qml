import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls
import Cara.Product

BackPage {
    id: root

    required property int productId

    title: "Historial de Precios"
    btnLoader.sourceComponent: ToolButton {
        icon.source: "qrc:/icons/add_circle.svg"
        onClicked: {
            newPriceDialog.open();
        }
    }

    Component.onCompleted: {
        priceHistoryRepository.fetchPriceHistoryBy(root.productId);
    }

    component PriceHistoryDelegate: Item {
        required property var model
        required property int price
        required property date startDate
        readonly property bool isCurrent: isNaN(model.thruDate) //=== null

        width: listView.width
        implicitHeight: 100

        Rectangle {
            anchors.fill: parent
            color: isCurrent ? "lightyellow" : "transparent"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            Label {
                text: `Price: ${price}`
            }
            Label {
                text: `Fecha de inicio: ${startDate.toLocaleDateString()}`
            }
            Label {
                text: `Fecha final: ${isCurrent ? "Precio actual" : model.thruDate.toLocaleDateString()}`
            }
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: PriceHistoryModel {
            id: priceHistoryModel
            repository: RestPriceHistoryRepository {
                id: priceHistoryRepository
                priceType: "sell"
            }
        }
        delegate: PriceHistoryDelegate {}
        section {
            property: "priceType"
            delegate: Rectangle {
                id: rectangle
                required property string section
                width: ListView.view.width
                height: childrenRect.height
                color: "red"

                Label {
                    padding: 12
                    text: rectangle.section
                    color: "white"
                    font.pointSize: 14
                }
            }
        }
    }

    Dialog {
        id: newPriceDialog
        width: root.width * .8
        height: root.height/2
        anchors.centerIn: parent
        title: "Establecer nuevo precio"
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            const newPrice = parseInt(newPriceField.text);
            if (newPrice < 0) {
                console.error("The inserted new price is less than 0.");
                return;
            }

            priceHistoryRepository.addNewPrice({productId, price: newPrice, price_type: "sell"});
        }

        TextField {
            id: newPriceField
            width: newPriceDialog.availableWidth
            anchors.verticalCenter: parent.verticalCenter
            placeholderText: "Nuevo precio RD$"
        }
    }
}
