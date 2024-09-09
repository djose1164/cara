import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Utils
import Cara.Billing
import "qrc:/provider"

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
                onClicked: stackView.pushItem(Qt.resolvedUrl("../Billing/CommissionPage.qml"), {commissionModel})
            }
        }
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
            }

            ProductSale {
                id: productSale
                enabled: providerSelector.currentIndex !== -1
                selectedProductsDelegate: ProfitProductDelegate {}
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
                title: "Seleccionar fecha de pago"
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

    ProfitCalculator {
        id: calculator
        percentage: spinBox.value
        model: productSale.choosenProducts
        selectorModel: productSale.productSelectorModel

        Binding on supplierId {
            when: !isNaN(providerSelector.currentValue)
            value: providerSelector.currentValue
        }
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
                                                                  : Math.min(root.width*.1, root.height*.1)
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
