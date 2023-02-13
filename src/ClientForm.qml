import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils"
import "qrc:/client"
import "qrc:/js/client_request.mjs" as Service

Page {
    id: root

    property string imgURL: "qrc:/icons/done.svg"
    readonly property StackView stackView_: StackView.view

    signal clientAdded

    header: PageHeader {
        title: "Nuevo cliente"
        stackView: stackView_
    }

    GridLayout {
        width: parent.width * .9
        anchors.centerIn: parent
        columns:  2
        rowSpacing: 30

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

        TextField {
            id: phoneNumber
            placeholderText: "Numero telefonico"
            Layout.fillWidth: true
        }

        TextField {
            id: country
            placeholderText: "Pais"
            text: "Republica Dominicana"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        TextField {
            id: province
            placeholderText: "Provincia"
            text: "Monseñor Nouel"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        TextField {
            id: municipality
            placeholderText: "Municipio"
            text: "Bonao"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        TextField {
            id: street
            placeholderText: "Calle"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        TextField {
            id: house_num
            placeholderText: "Numero de casa"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }
    }

    footer: Button {
        text: "Ok"
        onClicked: {
            busyIndicator.running = true
            const client = {
                "forename": forename.text,
                "surname": surname.text,
                "phone_number": phoneNumber.text,
                "address": {
                    "country": country.text,
                    "province": province.text,
                    "municipality": municipality.text,
                }
            };
            if (street.length)
                client.address["street"] = street.text;
            if (house_num.length)
                client.address["house_num"] = house_num.text;
            let xhr = new Service.ClientRequest("https://cara-api.azurewebsites.net/api");
            xhr.createClient(client, );
        }
    }

    ClientModel {
        id: clientModel
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    MessageDialog {
        id: messageDialog
        buttons: MessageDialog.Ok
        onButtonClicked: close()
        title: "Crear cliente"

        Image {
            source: imgURL
        }
    }

    function clientCreated(response) {
        busyIndicator.running = false;
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
