import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Popup {
    id: newProviderForm

    anchors.centerIn: Overlay.overlay
    modal: true

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        ScrollView {
            ScrollBar.horizontal.interactive: false
            ColumnLayout {
                id: fieldsColumn
                readonly property var provider: {
                    "name": providerName.text,
                    "contact": contactForm.contact,
                }

                width: newProviderForm.width
                spacing: 20

                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    text: "Nuevo Proveedor"
                    font.bold: true
                    font.pointSize: 14
                    horizontalAlignment: Label.AlignHCenter
                }

                TextField {
                    id: providerName
                    Layout.fillWidth: true
                    placeholderText: "Nombre de proveedor..."
                }

                ContactForm {
                    id: contactForm
                    Layout.fillWidth: true
                }

                Button {
                    onClicked: {
                        print(JSON.stringify(fieldsColumn.provider, null, 2));
                        stackLayout.currentIndex = 1;
                        api.createProvider(fieldsColumn.provider);
                    }

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    text: "Agregar"
//                    Material.background: Material.Green
//                    Material.foreground: "white"
                }
            }
        }

        BusyIndicator {}

        Item {
            Label {
                anchors.centerIn: parent
                text: "Error al agregar nuevo proveedor"
                font.pointSize: 14
            }
        }
    }

    ProviderAPI {
        id: api

        onFailed: stackLayout.currentIndex = 2
        onProviderCreated: {
            listModel.append(fieldsColumn.provider);
            newProviderForm.close();
        }
    }
}
