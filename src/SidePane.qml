import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/users"

Drawer {
    id: root
    required property StackView stackView
    width: {
        print("Drawer: parent.width: " + parent.width)
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

        Label {
            text: "Menu"
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Loader { sourceComponent: TheCurrentUser.isGuest ? guest : logged }

        Label {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.bottomMargin: 10
            horizontalAlignment: Label.AlignHCenter
            text: "Para cualquier sugerencia o reporte de errores, escribenos a: "
            + "[djsoe1164@gmail.com](mailto:djsoe1164@gmail.com)"
            wrapMode: Label.WordWrap
            textFormat: Label.MarkdownText
            onLinkActivated: url => Qt.openUrlExternally(url)
            fontSizeMode: Label.Fit
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
            width: root.width
            height: root.height/2
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 15

            Label {
                text: "Informacion Personal"
                font.pointSize: 12
                font.bold: true
            }

            Label {
                text: "Usario: " + TheCurrentUser.username
                font.pointSize: 11
                visible: TheCurrentUser.username
            }

            Label {
                text: "Nombre"
                font.pointSize: 11
                font.bold: true
            }

            Label {
                text: TheCurrentUser.forename + " " + TheCurrentUser.surname
                font.pointSize: 11
            }

            Label {
                Layout.fillWidth: true
                text: "**Correo electronico**"
                textFormat: Label.MarkdownText
                font.pointSize: 11
            }

            Label { text: TheCurrentUser.email }

            Button {
                Layout.alignment: Qt.AlignBottom
                text: "Cerrar sesion"
                flat: true
                onClicked: {
                    close();
                    TheCurrentUser.clear();
                    stackView.replace("qrc:/main/MainPage.qml");
                }
            }
        }
    }
}
