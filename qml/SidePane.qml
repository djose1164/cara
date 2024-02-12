import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/singleton"
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

        Label {
            text: "Menu"
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Loader { sourceComponent: isGuest ? guest : logged }

        ToolSeparator {
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }

        Label {
            text: "Contactanos"
            font.pointSize: 12
            font.bold: true
        }

        Label {
            text: "[Twitter](https://twitter.com/carardoficial)"
            font.pointSize: 11
            textFormat: Label.MarkdownText
            onLinkActivated: url => Qt.openUrlExternally(url)
        }

        Label {
            text: "[Correo electronico](mailto:djsoe1164@gmail.com)"
            font.pointSize: 11
            textFormat: Label.MarkdownText
            onLinkActivated: url => Qt.openUrlExternally(url)
        }

        ToolSeparator {
            orientation: Qt.Horizontal
            visible: LoginManager.status == LoginManager.LoggedIn
            Layout.fillWidth: true
        }

        Button {
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 15
            text: "Cerrar sesion"
            flat: true
            visible: !isGuest
            onClicked: {
                close();
                UserSettings.clear();
                SalespersonSettings.clear();
                LoginManager.logOut();
            }
        }
    }

    Component {
        id: guest
        ColumnLayout {
            Layout.fillWidth: true

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
            Layout.fillWidth: true
            spacing: 20

            Label {
                text: "Informacion Personal"
                font.pointSize: 12
                font.bold: true
            }

            Label {
                text: user.forename + " " + user.surname
                font.pointSize: 11
            }

            Button {
                text: "Ver perfil"
                flat: true
                onClicked: {
                    close();
                    appStack.push("qrc:/qt/qml/User/PerfilPage.qml");
                }
            }
        }
    }
}
