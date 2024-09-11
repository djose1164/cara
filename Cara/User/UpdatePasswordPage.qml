import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Cara.Controls
import Cara.User
import Cara.Authentication

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

        PasswordItem {
            id: passwordItem
        }

        Button {
            id: btn
            width: column.width
            enabled: passwordItem.acceptable
            text: "Aceptar"
            onClicked: {
                const payload = {
                    password: {
                        current_password: currentPassword.text,
                        new_password: passwordItem.text
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
