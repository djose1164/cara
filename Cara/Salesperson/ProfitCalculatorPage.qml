pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Controls
import Cara.Billing
import Cara.ProductInventory
import "commission_logic.js" as Logic

BackPage {
    id: root

    required property int adminId

    title: qsTr("Calculadora de Ganancias")
    btnLoader.sourceComponent: MoreVertButton {
        onClicked: menu.open()

        Menu {
            id: menu
            MenuItem {
                text: "Ver historial"
                onTriggered: {
                    root.stackView.pushItem(Qt.resolvedUrl("../Billing/CommissionPage.qml"),
                                            {commissionController})}
            }

            MenuItem {
                text: "Ayuda"
                icon.name: "help"
                onTriggered: helpDialog.open()
            }
        }
    }

    MessageDialog {
        id: helpDialog
        title: "Ayuda"
        text: "Los precios listados en 'Productos agregados' corresponde a la"
              + " ganancia neta por unidad de ese producto. Para calcular dicha"
              + " ganancia neta se utiliza la siguiente formula:"
              + " <b>Ganancia Neta = Precio de Venta - Precio de Compra</b>"
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: columnLayout.implicitHeight + internal.marginSize

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            anchors.margins: internal.marginSize
            spacing: 20

            SellerSelector {
                id: sellerSelector
                sellerModel: commissionController.sellerModel
                Layout.fillWidth: true
            }

            ProviderComboBox {
                id: providerSelector
                Layout.fillWidth: true
                onActivated: {
                    if (selected) commissionController.productModel.update()
                }
            }

            ProductSale {
                id: productSale
                enabled: internal.productSelectionEnabled
                selectorModel: commissionController.productModel
                selectedProductsDelegate: ProfitProductDelegate {
                    width: productSale.implicitViewWidth
                    onDeleted: (idx, id) => productSale.selectedProductsView.deleteClicked(idx, id)
                }

                selectorDelegate: ProductSelectorDelegate {
                    required model
                    required productId

                    width: root.width
                    height: 90
                    imageUrl: model.imageUrl
                    text: `<h3>${model.productName}</h3>`
                          + `${model.description}`
                    productModel: productSale.selectorModel
                    selection: productSale.selectionModel
                }

                Layout.fillWidth: true
                Layout.minimumHeight: productSale.productSelectionModel.count > 0 ? root.height * .4 : productSale.implicitHeight
            }

            RedirectLabel {
                title: datePicker.isValid ? datePicker.dateString
                                          : "Seleccionar fecha de pago"
                Layout.fillWidth: true
                onClicked: datePicker.open()
            }

            DatePicker {
                id: datePicker
            }

            Label {
                text: "Total de Ganancia Neto: <b>RD$%1</b>".arg(calculator.grossTotal)
                font.pointSize: 14
                wrapMode: Label.Wrap
                Layout.fillWidth: true
            }

            CommissionSummary {
                id: commissionSummary
                total: calculator.total
                Layout.fillWidth: true
            }

            Button {
                text: "Guardar"
                Material.background: Material.Blue
                Material.foreground: "white"
                Layout.fillWidth: true
                onClicked: root.saveSale()
            }

            Item { implicitHeight: 1 }
        }
    }

    CommissionController {
        id: commissionController
        adminId: root.adminId
        supplierId: providerSelector.currentValue || 0
        onCommissionAdded: messageDialog.show("Exito", "Comision guardada.")
        onErrorOccurred: (errcode, errstr) => {
            messageDialog.show(`Codigo de error ${errcode}`, `Detalles: ${errstr}`);
        }
    }

    ProfitCalculator {
        id: calculator
        percentage: commissionSummary.commissionRate * 100
        model: productSale.productSelectionModel
    }

    MessageDialog {
        id: messageDialog

        function show(title: string, text: string) {
            messageDialog.title = title;
            messageDialog.text = text;
            messageDialog.open();
        }
    }

    QtObject {
        id: internal
        readonly property double marginSize: root.width < 401
                                             && root.height < 641 ? Math.max(scrollView.effectiveScrollBarWidth,
                                                                             Math.min(root.width*.025, root.height*.025))
                                                                  : Math.min(root.width*.07, root.height*.07)
        readonly property bool productSelectionEnabled: providerSelector.currentIndex !== -1
                                                        && commissionController.productModel.count > 0
    }

    function saveSale() {
        try {
            Logic.validateForm(sellerSelector, providerSelector, productSale);
            commissionController.saveCommission(root.toJson());
        } catch (err) {
            messageDialog.show("Error", err);
            console.log(err);
        }
    }

    function toJson() {
        return {
            adminId,
            rate: commissionSummary.commissionRate,
            salespersonId: sellerSelector.sellerId,
            paymentDate: datePicker.selectedDate,
            commissionItems: Logic.buildCommissionItems(productSale, providerSelector.currentValue),
        };
    }
}
