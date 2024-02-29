import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Salesperson
import User

Drawer {
    id: root

    required property var user

    required property StackView stackView
    readonly property bool isGuest: user.userType === UserEntity.Guest

    width: {
        if (parent.width >= 200 && parent.width <= 500)
            return parent.width * .65;
        if (parent.width > 500)
            return parent.width * .4
        else
            return parent.width * .8
    }
    height: parent.height

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
                close();
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
                    appStack.push("qrc:/qt/qml/Authentication/LoginPage.qml");
                }
                text: "Iniciar Sesion"
                flat: true
            }

            Button {
                onClicked: {
                    close();
                    stackView.push("qrc:/qt/qml/Authentication/SignUp.qml");
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
                icon.source: "qrc:/qt/qml/Customer/person.svg"
                flat: true
                onClicked: {
                    close();
                    appStack.push("qrc:/qt/qml/User/PerfilPage.qml");
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
