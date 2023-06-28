import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/singleton"
import com.cara

Drawer {
    id: root

    required property StackView stackView
    readonly property UserEntity user: CurrentUser.user
    readonly property bool isGuest: user.userType === UserEntity.GUEST

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
        anchors.leftMargin: 15
        Label {
            text: "Menu"
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Loader { sourceComponent: isGuest ? guest : logged }

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

        Button {
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 15
            text: "Cerrar sesion"
            flat: true
            visible: LoginManager.status == LoginManager.LoggedIn
            onClicked: {
                close();
                TheCurrentUser.clear();
                LoginManager.logOut()
                // stackView.replace("qrc:/main/MainPage.qml");
            }
        }
    }

    Component {
        id: guest
        ColumnLayout {
            Layout.fillWidth: true

            Button {
                onClicked: stackView.replace("qrc:/login/Login.qml")
                text: "Iniciar Sesion"
                flat: true
            }

            Button {
                onClicked: stackView.replace("qrc:/login/SignUp.qml")
                text: "Registrarse"
                flat: true
            }
        }
    }

    Component {
        id: logged
        ColumnLayout {
            Layout.fillWidth: true
            Label {
                text: "Informacion Personal"
                font.pointSize: 12
                font.bold: true
            }

            Label {
                text: "Usario: " + user.username
                font.pointSize: 11
                visible: CurrentUser
            }

            Label {
                text: "Nombre"
                font.pointSize: 11
                font.bold: true
            }

            Label {
                text: user.forename + " " + user.surname
                font.pointSize: 11
            }

            Label {
                Layout.fillWidth: true
                text: "**Correo electronico**"
                textFormat: Label.MarkdownText
                font.pointSize: 11
            }

            Label {
                Layout.preferredWidth: menuLayout.width
                text: user.email
                wrapMode: Label.Wrap
            }
        }
    }
}
