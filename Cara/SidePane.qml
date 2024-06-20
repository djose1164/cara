import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Salesperson
import Cara.User
import Cara.Authentication

Drawer {
    id: root

    required property StackView stackView
    readonly property bool isGuest: UserSettings.userTypeId === UserEntity.Guest

    width: {
        if (implicitWidth >= 200 && implicitWidth <= 500)
            return implicitWidth * .65;
        if (implicitWidth > 500)
            return implicitWidth * .4
        else
            return implicitWidth * .8
    }

    ColumnLayout {
        id: menuLayout
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 10

        Label {
            text: "Menu"
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Loader {
            sourceComponent: isGuest ? guest : logged
            Layout.fillWidth: true
        }

        ToolSeparator {
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }

        ContactUs {
            Layout.fillWidth: true
        }

        ToolSeparator {
            visible: logoutBtn.visible
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }

        Button {
            id: logoutBtn
            visible: !isGuest
            text: "Cerrar sesion"
            flat: true
            icon.source: "logout.svg"
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 15
            onClicked: {
                UserSettings.clear();
                SalespersonSettings.clear();
                LoginManager.logOut();
                root.close();
            }
        }

        Label {
            id: version
            text: `Version: <b>${Qt.application.version}</b>`
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
        }
    }

    Component {
        id: guest
        ColumnLayout {
            Button {
                onClicked: {
                    close();
                    appStack.push("qrc:/qt/qml/Cara/Authentication/LoginPage.qml");
                }
                text: "Iniciar Sesion"
                flat: true
            }

            Button {
                onClicked: {
                    close();
                    stackView.push("qrc:/qt/qml/Cara/Authentication/SignUp.qml");
                }
                text: "Registrarse"
                flat: true
            }
        }
    }

    Component {
        id: logged
        ColumnLayout {
            implicitHeight: 150
            spacing: 10

            Button {
                text: "Ver perfil"
                icon.source: "qrc:/qt/qml/Cara/Customer/person.svg"
                flat: true
                onClicked: {
                    close();
                    appStack.push("qrc:/qt/qml/Cara/User/PerfilPage.qml");
                }
            }

            Button {
                icon.source: "qrc:/qt/qml/Cara/Billing/pdf-icon.svg"
                text: "Facturas exportadas"
                flat: true
                onClicked: {
                    close();
                    stackView.pushItem("qrc:/qt/qml/Cara/Billing/PdfBillListing.qml")
                }
            }

            Button {
                text: "Configuraciones"
                icon.source: "qrc:/icons/settings.svg"
                flat: true
                onClicked: {
                    close();
                    stackView.pushItem("qrc:/qt/qml/Cara/SettingsPage.qml");
                }
            }
        }
    }
}
