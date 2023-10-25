import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core
import "qrc:/utils"
import "qrc:/customers"
import "qrc:/singleton"
import "qrc:/main"

Page {
    id: root

    property string imgURL: "qrc:/icons/done.svg"
    readonly property StackView stackView_: StackView.view

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

            NameForm {
                id: nameForm
            }

            ContactForm {
                id: contactForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            Button {
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignCenter
                text: "Agregar Cliente"
                onClicked: addCustomer()
            }
        }
    }

    CustomerModel { id: customerModel }

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
        busyIndicator.open();

        contactForm.removeEmptyFields();
        const client = {
            "forename": nameForm.forename,
            "surname": nameForm.surname,
            "contact": contactForm.contact,
            "admin_id": CurrentUser.user.userId
        };

        clientRequest.create(client);
    }

    function customerCreated(customer) {
        CustomerManager.addCustomer(customer);
        busyIndicator.close();

        messageDialog.text = "Cliente agregado con exito!";
        messageDialog.open();
    }
}
