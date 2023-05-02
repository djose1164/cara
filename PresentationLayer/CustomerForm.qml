import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils"
import "qrc:/customers"
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

    GridLayout {
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        columns:  2
        rowSpacing: 20

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

        Button {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignCenter
            text: "Ok"
            onClicked: {
                busyIndicator.open();
                const client = {
                    "forename": forename.text,
                    "surname": surname.text,
                    "telephone": phoneNumber.text ? phoneNumber.text : null,
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
                clientRequest.create(client);
            }
        }
    } // GridLayout

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
