pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Controls
import Cara.ProductInventory

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
        id: priceDelegate
        required property var model
        required property int price
        required property date startDate
        readonly property bool isCurrent: isNaN(model.thruDate) //=== null

        width: listView.width
        implicitHeight: 100
        onIsCurrentChanged: if (isCurrent) internal.latestPrice = priceDelegate.price

        Rectangle {
            anchors.fill: parent
            color: priceDelegate.isCurrent ? "lightyellow" : "transparent"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10

            Label {
                text: `Price: ${priceDelegate.price}`
            }

            Label {
                text: `Fecha de inicio: ${priceDelegate.startDate.toLocaleDateString()}`
            }

            Label {
                text: `Fecha final: ${priceDelegate.isCurrent ? "Precio actual"
                                                              : priceDelegate.model.thruDate.toLocaleDateString()}`
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
            onErrorOccurred: (error, errorString) => {
                const errorName = root._errorCodeToString();
                errorMessage.openWith(`Error code: ${error}<br>Error: ${errorName}`, errorString);
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

    PriceDialog {
        id: newPriceDialog
        title: "Establecer nuevo precio"
        oldPrice: internal.latestPrice
        onAccepted: {
            if (newPriceDialog.newPrice < 0) {
                errorMessage.openWith("Debes ingresar un precio mayor a 0.");
                console.error("The inserted new price is less than 0.");
                return;
            }

            priceHistoryRepository.addNewPrice({productId, price: newPriceDialog.newPrice,
                                                priceType: "sell"});
        }
    }

    MessageDialog {
        id: errorMessage
        title: "Error"

        function openWith(text: string, detail: string) {
            errorMessage.text = text;
            errorMessage.detailedText = detail;
            errorMessage.open()
        }
    }

    QtObject {
        id: internal
        property double latestPrice
    }

    function _errorCodeToString(errorCode: int): string {
        switch (errorCode) {
        case PriceHistory.FailedToAddNewPrice:
            return "Error al añadir nuevo precio";
        case PriceHistory.FailedToFetchPriceHistory:
            return "Error al leer historico de precios";
        default:
            return "Error desconocido: " + errorCode;
        }
    }
}
