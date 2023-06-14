import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Drawer {
    id: root

    Row {
        id: header
        anchors.top: root.top
        width: root.width
        height: 50

        CloseButton {
            onClicked: close()
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: "Actualizar Datos"
            font.pointSize: 12
            font.bold: true
        }
    }

    ScrollView {
        id: content
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            leftMargin: 15
        }
        height: root.height - header.height
        contentWidth: availableWidth

        ColumnLayout {
            anchors.fill: parent
            anchors.rightMargin: 15
            GroupBox {
                title: "Informacion Personal"
                Layout.fillWidth: true
                RowLayout {
                    anchors.fill: parent
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
            }

            ContactForm {
                id: contactForm
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }

            Button {
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignCenter
                text: "Actualizar"
                onClicked: {
                    busyIndicator.open();

                    contactForm.removeEmptyFields();
                    const client = {
                        "forename": forename.text,
                        "surname": surname.text,
                        "contact": contactForm.contact,
                    };

                    clientRequest.create(client);
                }
            }
        }
    }
}
