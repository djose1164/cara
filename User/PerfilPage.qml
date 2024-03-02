import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Utils
import User
import Salesperson

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    title: "Perfil"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
        btnLoader.sourceComponent: ToolButton {
            text: "Guardar"
        }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: root.width * .95
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                text: "Nombre"
            }

            TextField {
                id: nameField
                text: UserSettings.forename
                Layout.fillWidth: true
            }

            Label {
                text: "Apellido"
            }

            TextField {
                id: lastNameField
                text: UserSettings.surname
                Layout.fillWidth: true
            }

            Label {
                text: "Correo electronico"
            }

            TextField {
                id: emailField
                text: UserSettings.email
                Layout.fillWidth: true
            }

            Column {
                Layout.fillWidth: true
                Label {
                    text: "Contraseña"
                }

                TextField {
                    id: passwordField
                    width: parent.width
                    text: "Contraseña"
                    echoMode: TextField.Password
                }
            }
            // TODO: Show salesperson details of this user.
            GroupBox {
                title: "Credito"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 15
                    enabled: false

                    TextField {
                        placeholderText: "Disponible"
                        text: SalespersonSettings.creditAvailable
                    }

                    TextField {
                        placeholderText: "Consumido"
                        text: SalespersonSettings.creditConsumed
                    }

                    TextField {
                        placeholderText: "Limite"
                        text: SalespersonSettings.creditLimit
                    }
                }
            }
        }
    }
}
