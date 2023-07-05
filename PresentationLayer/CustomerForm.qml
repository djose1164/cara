import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import com.cara
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
        stackView: stackView_
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15

        RowLayout {
            TextField {
                id: forename
                placeholderText: "Nombre"
                Layout.fillWidth: true
            }

            TextField {
                id: surname
                placeholderText: "Apellido"
                Layout.fillWidth: true
            }
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
            onClicked: {
                busyIndicator.open();

                contactForm.removeEmptyFields();
                const client = {
                    "forename": forename.text,
                    "surname": surname.text,
                    "contact": contactForm.contact,
                    "admin_id": CurrentUser.user.userId
                };

                clientRequest.create(client);
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

    ClientRequest {
        id: clientRequest
        request.onSucceeded: r => clientCreated(r)
    }

    function clientCreated(response) {
        CustomerManager.updateModel();
        busyIndicator.close();

        if (response.code === "success") {
            messageDialog.text = "Cliente agregado con exito!";
        } else {
            messageDialog.text = "No se ha podido completar la accion."
            messageDialog.informativeText = "Comprueba coneccion a internet y/o "
                    + "datos obligatorios.";
            imgURL = "qrc:/icons/error.svg"
        }

        messageDialog.open()
    }
}
