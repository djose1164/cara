import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Drawer {
    property string imgURL: "qrc:/icons/done.svg"

    id: drawer
    width: parent.width
    height: parent.height * .9
    edge: Qt.BottomEdge

    Page {
        anchors.fill: parent
        header: Label {
            text: "Nuevo cliente"
            font.pointSize: 20
            font.bold: true
            horizontalAlignment: Label.AlignHCenter
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
                worker.sendMessage({"type": "createClient", "client": client});
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        running: false
    }

    MessageDialog {
        id: messageDialog
        buttons: MessageDialog.Ok
        onButtonClicked: close()
        title: "Crear cliente"

        Image {
            anchors.fill: parent
            source: imgURL
        }
    }

    WorkerScript {
        id: worker
        source: "qrc:/js/client_request.mjs"
        onMessage: (response) => {
                       busyIndicator.running = false;
                       if (response.code === "success")
                            messageDialog.text = "Cliente agregado con exito!"
                       else {
                           messageDialog.text = "No se ha podido completar la accion."
                           messageDialog.informativeText = "Comprueba coneccion a internet y/o "
                                                           + "datos obligatorios.";
                            imgURL = "qrc:/icons/error.svg"
                       }
                       messageDialog.open()
                   }
    }
}
