import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.User
import Cara.Salesperson
import Cara.Controls
import Cara.Customer

Page {
    id: root

    property string imgURL: "qrc:/icons/done.svg"
    readonly property T.StackView stackView_: StackView.view
    readonly property LocalUserStore userStore: LocalUserStore {}

    signal clientAdded

    header: TitleHeader {
        title: "Nuevo cliente"
        onBackClicked: stackView_.pop()
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: root.width*.9
            anchors.centerIn: parent

            PersonalInformationForm {
                id: nameForm
            }

            ContactForm {
                id: contactForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            AddressForm {
                id: addressForm
                Layout.fillWidth: true
            }

            Button {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.bottomMargin: 15
                text: "Agregar Cliente"
                onClicked: addCustomer()
            }
        }
    }

    BusyPopup { id: busyIndicator }

    MessageDialog {
        id: messageDialog
        buttons: MessageDialog.Ok
        onButtonClicked: close()
        title: "Crear cliente"

        Image { source: imgURL }
    }

    CustomerAPI {
        id: clientRequest
        onCustomerCreated: customer => root.customerCreated(customer)
        rest.onFailed: function(resp) {
            busyIndicator.close();
            messageDialog.text = "Error al crear cliente";
            messageDialog.informativeText = resp.message;
            messageDialog.open();
        }
    }

    function addCustomer(): void {
        try {
            validate();
            busyIndicator.open();

            let person = {
                contact: contactForm.toJson(),
                address: addressForm.address
            };

            Object.assign(person, nameForm.toJson());
            const client = {
                person,
                "salesperson_id": SalespersonSettings.salespersonId
            };

            clientRequest.create(client);
        } catch (err) {
            console.error(err);
            messageDialog.text = "Datos invalidos";
            messageDialog.informativeText = err;
            messageDialog.open();
        }
    }

    function customerCreated(customer) {
        CustomerManager.addCustomer(customer);
        busyIndicator.close();

        messageDialog.text = "Cliente agregado con exito!";
        messageDialog.informativeText = "";
        messageDialog.open();
    }

    function validate(): void {
        nameForm.validate();
        // contactForm.validate();
        // addressForm.validate();
    }
}
