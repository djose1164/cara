import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import User
import Salesperson
import "qrc:/utils"
import Customer
import "qrc:/main"

Page {
    id: root

    property string imgURL: "qrc:/icons/done.svg"
    readonly property StackView stackView_: StackView.view
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

            contactForm.removeEmptyFields();
            const contact = {
                address: addressForm.address,
                "forename": nameForm.forename,
                "surname": nameForm.surname,
            };
            Object.assign(contact, contactForm.contact);
            const client = {
                "contact": contact,
                "salesperson_id": SalespersonSettings.salespersonId
            };

            clientRequest.create(client);
        } catch (err) {
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
