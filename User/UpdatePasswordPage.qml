import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Utils
import User

BackPage {
    id: root

    title: "Actualizar contraseña"

    Column {
        id: column
        anchors.fill: parent
        anchors.margins: 10
        spacing: 35

        PasswordInput {
            id: currentPassword
            width: column.width
            placeholderText: "Contraseña actual"
            eyeBtn.visible: false
        }

        PasswordInput {
            id: firstPassword
            width: column.width
            placeholderText: "Nueva contraseña"
        }

        PasswordInput {
            id: secondPassword
            width: column.width
            placeholderText: "Confirmar contraseña"
            ToolTip {
                x: secondPassword.leftPadding
                y: secondPassword.height
                width: secondPassword.width * .8
                visible: firstPassword.text !== secondPassword.text
                text: "Las contraseñas no coinciden"
            }
        }

        Button {
            id: btn
            width: column.width
            enabled: firstPassword.text && firstPassword.text === secondPassword.text
            text: "Aceptar"
            onClicked: {
                const payload = {
                    password: {
                        current_password: currentPassword.text,
                        new_password: firstPassword.text
                    }
                };
                userController.updatePassword(UserSettings.userId, payload);
            }
        }
    }

    MessageDialog {
        id: message
    }

    ProcessingIndicator {
        id: busyIndicator
    }

    UserController {
        id: userController
        onPasswordUpdated: {
            message.title = "Contraseña actualizada"
            message.text = "";
            message.open();
            root.clear();
        }
        onErrorOccurred: {
            message.title = "Error";
            message.text = errorString;
            message.open();
        }
    }

    function clear() {
        currentPassword.text = "";
        firstPassword.text = "";
        secondPassword.text = "";
    }
}
