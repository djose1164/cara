pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Controls
import Cara.Billing
import Cara.ProductInventory

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
                onTriggered: root.stackView.pushItem(Qt.resolvedUrl("../Billing/CommissionPage.qml"), {commissionModel})
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

            SalespeopleSelector {
                id: spSelector
                adminId: root.adminId
                Layout.fillWidth: true
            }

            ProviderComboBox {
                id: providerSelector
                Layout.fillWidth: true
                onActivated: {
                    if (selected) productModel.update()
                }
            }

            ProductSale {
                id: productSale
                enabled: providerSelector.currentIndex !== -1
                         && productModel.count > 0
                selectorModel: productModel
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
                    productModel: productSale.productSelectorModel
                    selection: productSale.selectionModel
                    onClicked: console.log("selectorDelegate.productId:",productId)
                }

                Layout.fillWidth: true
                Layout.minimumHeight: productSale.choosenProducts.count > 0 ? root.height * .4 : productSale.implicitHeight
            }

            Label {
                text: "Total de Ganancia Neto: <b>RD$%1</b>".arg(calculator.grossTotal)
                font.pointSize: 14
                wrapMode: Label.Wrap
                Layout.fillWidth: true
            }

            GroupBox {
                title: "Ganancias"
                Layout.fillWidth: true
                ColumnLayout {
                    id: gbColumnLayout
                    anchors.fill: parent
                    spacing: 20

                    RowLayout {
                        spacing: 20
                        Layout.fillWidth:true

                        SpinBox {
                            id: spinBox
                            from: 1
                            value: 5
                            stepSize: 5
                            editable: true
                            validator: IntValidator {}
                        }

                        Label {
                            text: "% de ganancia del vendedor"
                            font.pointSize: 12
                            wrapMode: Label.Wrap
                            Layout.fillWidth: true
                        }
                    }

                    Label {
                        text: "Total de Ganancia Neto - % de ganancia: <b>$RD%1</b>"
                        .arg(calculator.total.toFixed(2))
                        font.pointSize: 12
                        wrapMode: Label.Wrap
                        Layout.fillWidth: true
                    }
                }
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

    ProductModel {
        id: productModel
        repository: SupplierProductRepository {
            supplierId: providerSelector.currentValue || 0
        }
        onUpdated: console.log("find(35):",find(35))
    }

    ProfitCalculator {
        id: calculator
        percentage: spinBox.value
        model: productSale.choosenProducts
    }

    CommissionModel {
        id: commissionModel
        repository: RestCommissionRepository {}
        onCommissionAddedSuccessfully: messageDialog.show("Exito", "Comision guardada.")
    }

    MessageDialog {
        id: messageDialog

        function show(title: string, text: string) {
            messageDialog.title = title;
            messageDialog.text = text;
            messageDialog.open();
        }
    }

    BusyPopup {
        id: busyPopup
        visible: commissionModel.busy && root.isPageVisible
    }

    QtObject {
        id: internal
        readonly property double marginSize: root.width < 401
                                             && root.height < 641 ? Math.max(scrollView.effectiveScrollBarWidth,
                                                                             Math.min(root.width*.025, root.height*.025))
                                                                  : Math.min(root.width*.07, root.height*.07)
    }

    function saveSale() {
        try {
            root.validateForm();
            commissionModel.append({
                adminId,
                rate: spinBox.value/100,
                salespersonId: spSelector.currentValue,
                paymentDate: datePicker.selectedDate,
                commissionItems: root.getCommissionItems()
            });
        } catch (err) {
            messageDialog.show("Error", err);
            console.log(err);
        }
    }

    function validateForm() {
        if (!spSelector.selected)
            throw "Necesitas seleccionar un vendedor";
        else if (!providerSelector.selected)
            throw "Necesitas seleccionar un suplidor";
        productSale.validate();
    }

    function getCommissionItems() {
        const items = productSale.toJson();
        items.forEach(item => item.supplier_id = providerSelector.currentValue);
        return items;
    }
}
