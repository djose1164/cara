pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Controls
import Cara.Billing
import Cara.ProductInventory

Item {
    id: root

    required property var model // Product type
    readonly property T.StackView stackView: StackView.view

    signal refreshRequested(productId: int)

    implicitWidth: 180
    implicitHeight: 220

    component SupplierDelegate: Item {
        id: supplierDelegate

        required property var model
        readonly property date startDate: supplierDelegate.model.startDate
        readonly property date thruDate: supplierDelegate.model.thruDate
        readonly property size iconSize: Qt.size(50, 50)

        implicitWidth: 140
        implicitHeight: frame.implicitHeight + frame.bottomPadding

        Frame {
            id: frame
            anchors.fill: parent
            anchors.margins: 6

            RowLayout {
                anchors.fill: parent
                spacing: 20
                // rows: 2
                // columns: 3

                Image {
                    source: "qrc:/qt/qml/Cara/Billing/business.svg"
                    sourceSize: supplierDelegate.iconSize
                    //Layout.rowSpan: 2
                }

                Label {
                    text: `<h2>${supplierDelegate.model.supplierName}</h2>
                           <span>Precio: <b>RD$${supplierDelegate.model.price}</b></span>
                           <br><span>Fecha de inicio: <b>${!isNaN(startDate) ? startDate.toLocaleDateString() : "invalido"}</b></span>
                           <br><span>Fecha de fin: <b>${!isNaN(thruDate) ? thruDate.toLocaleDateString() : "Precio actual"}</b></span>`
                    wrapMode: Label.Wrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                Image {
                    source: isNaN(supplierDelegate.thruDate) ? "qrc:/qt/qml/Cara/Billing/current_price.png" : "qrc:/qt/qml/Cara/Billing/previous_price.png"
                    sourceSize: supplierDelegate.iconSize
                }
            }
        }
    }

    component ViewHeader: TitleHeader {
        width: root.width
        z: 2
        onBackClicked: root.stackView.popCurrentItem()
        title: `<h4>Actualizar suplidor</h4>
                <font size="-1">${root.model.productName}</font>`
        btnLoader.sourceComponent: ToolButton {
            icon.source: "qrc:/icons/add_circle.svg"
            onClicked: priceDialog.open()
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        header: ViewHeader {}
        headerPositioning: ListView.OverlayHeader
        model: root.model.buyPriceHistory
        delegate: SupplierDelegate {
            width: ListView.view.width
        }
    }

    Label {
        width: root.width * .9
        anchors.centerIn: parent
        visible: listView.count === 0
        text: "El histórico de este producto está vacío."
        horizontalAlignment: Label.AlignHCenter
        wrapMode: Label.Wrap
        font.pointSize: 14
    }

    PriceDialog {
        id: priceDialog
        title: "Establecer nuevo precio"
        oldPrice: root.model.supplierPrice
        onAccepted: {
            const json = supplierCB.toJson();
            json.price = priceDialog.newPrice;
            json.productId = root.model.productId;
            json.priceType = "buy";
            console.log(JSON.stringify(json))
            repository.addNewPrice(json);
        }
        Component.onCompleted: {
            priceDialog.okButtonConditions.push(() => supplierCB.currentIndex >= 0);
        }

        ProviderComboBox {
            id: supplierCB
            currentIndex: root.model.supplierId - 1
            Layout.fillWidth: true
        }
    }

    RestPriceHistoryRepository {
        id: repository
        onErrorOccurred: (error, errorString) => {
            errorDialog.openWith(`Codigo de error: ${error}
                                 <br>Error: ${errorString}`);
        }

        onAdded: byteArray => {
            // const json = JSON.parse(byteArray);
            // const newPrice = {};
            // newPrice.priceId = json["id"];
            // newPrice.price = json["price"];
            // newPrice.startDate = json["start_date"];
            // newPrice.thruDate = json["thru_date"];
            // newPrice.supplierId = json["supplier"]["id"];
            // newPrice.supplierName = json["supplier"]["name"];
            // root.model.buyPriceHistory.append(newPrice);
            // TODO: We need to append to PriceHistoryModel instead emitting signal.
            console.log("Emittig refreshRequested from UpdateSupplierView. productId: "+root.model.productId)
            root.refreshRequested(root.model.productId);
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error"

        function openWith(text: string) {
            errorDialog.text = text;
            errorDialog.open();
        }
    }
}
