import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Salesperson
import Cara.Controls
import QtQuick.Templates as T

Page {
    id: root

    required property int salespersonId
    required property int creditLimit
    required property int creditAvailable
    required property int creditConsumed
    required property var user

    readonly property T.StackView stackView: StackView.view

    title: "Detalles de vendedor"
    header: TitleHeader {
        title: root.title
        backBtn.onClicked: stackView.pop()
        btnLoader.sourceComponent: MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu

                Repeater {
                    model: listModel
                    MenuItem {
                        required text
                        required property string source
                        onTriggered: stackView.push(source, {salespersonId})
                    }
                }

                ListModel {
                    id: listModel

                    ListElement {
                        text: "Ver clientes"
                        source: "qrc:qt/qml/Cara/Customer/Customers.qml"
                    }

                    // ListElement {
                    //     text: "Ver estadisticas"
                    //     source: "qrc:/statistics/Statistics.qml"
                    // }

                    ListElement {
                        text: "Ver inventario"
                        source: "qrc:/inventory/Inventory.qml"
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 20

        Label {
            text: user.forename +" "+ user.surname
            font.pointSize: 12
            Layout.columnSpan: 2
        }

        TextField {
            id: limitField
            text: creditLimit
            placeholderText: "Limite de credito actual"
            validator: IntValidator {}
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.fillWidth: true
        }

        TextField {
            id: availableField
            enabled: false
            text: creditAvailable
            placeholderText: "Credito disponible"
            validator: IntValidator {}
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.fillWidth: true
        }

        TextField {
            id: consumedField
            enabled: false
            text: creditConsumed
            placeholderText: "Credito consumido"
            validator: IntValidator {}
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.fillWidth: true
        }

        Button {
            enabled: creditLimit !== parseInt(limitField.text)
                     && parseInt(limitField.text) > creditLimit
            text: "Actualizar"
            Material.background: "green"
            Layout.fillWidth: true
            onClicked: {
                try {
                    validate();
                    let payload = {};
                    const limit = parseInt(limitField.text);
                    if (creditLimit !== limit)
                        payload.credit_limit = limit;

                    const available = parseInt(availableField.text);
                    if (creditAvailable !== available)
                        payload.credit_available = available;

                    spService.patch(payload);
                } catch(e) {
                    print(e)
                    print(typeof e)
                    message.text = e;
                    message.open();
                }
            }
        }
    }
    
    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onButtonClicked: close()
    }
    
    SalespersonController {
        id: spService
        salespersonId: root.salespersonId
        onPatched: {
            const cl = parseInt(limitField.text);
            creditAvailable = cl - creditConsumed;
            creditLimit = cl;

            message.text = "Datos actualizados con exito!"
            message.open();
        }
    }
    
    function validate(): void {
        if (creditAvailable > creditLimit)
            throw "La cantidad de credito disponible no puede exceder el limit de credito del vendedor.";
    }
}
