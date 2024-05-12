import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtTest
import Cara.Authentication
import Cara

TestCase {
    name: "Signup"
    width: 400
    height: 620

    property SignUp signup
    readonly property string msgTemplate: "El campo de %1 no puede estar vacio."

    Component {
        id: component
        SignUp {
            width: 400
            height: 620
        }
    }

    function init() {
        signup = createTemporaryObject(component, parent);
    }

    function test_shouldShowDialogIfNameIsEmpty() {
        const forenameField = findChild(signup, "forenameField") as TextField;
        const signUpBtn = findChild(signup, "signUpBtn") as Button;
        const messageDialog = findChild(signup, "messageDialog") as MessageDialog;

        verify(forenameField);
        verify(signUpBtn);
        verify(messageDialog);

        mouseClick(signUpBtn);

        verify(messageDialog.text === msgTemplate.arg("nombre"));
    }

    function test_shouldShowDialogIfSurnameIsEmpty() {
        const forenameField = findChild(signup, "forenameField") as TextField;
        const surnameField = findChild(signup, "surnameField") as TextField;
        const signUpBtn = findChild(signup, "signUpBtn") as Button;
        const messageDialog = findChild(signup, "messageDialog") as MessageDialog;

        verify(forenameField);
        verify(surnameField);
        verify(signUpBtn);
        verify(messageDialog);

        forenameField.text = "juan";
        mouseClick(signUpBtn);

        verify(messageDialog.text === msgTemplate.arg("apellido"));
    }

    function test_shouldShowDialogIfUsernameIsEmpty() {
        const forenameField = findChild(signup, "forenameField") as TextField;
        const surnameField = findChild(signup, "surnameField") as TextField;
        const usernameField = findChild(signup, "usernameField");
        const signUpBtn = findChild(signup, "signUpBtn") as Button;
        const messageDialog = findChild(signup, "messageDialog") as MessageDialog;

        verify(forenameField);
        verify(surnameField);
        verify(usernameField);
        verify(signUpBtn);
        verify(messageDialog);

        forenameField.text = "juan";
        surnameField.text = "gonzalez";
        mouseClick(signUpBtn);

        verify(messageDialog.text === msgTemplate.arg("usuario"));
    }

    function test_shouldShowDialogIfPasswordIsEmpty() {
        const forenameField = findChild(signup, "forenameField") as TextField;
        const surnameField = findChild(signup, "surnameField") as TextField;
        const usernameField = findChild(signup, "usernameField");
        const passwordField = findChild(signup, "passwordField");
        const signUpBtn = findChild(signup, "signUpBtn") as Button;
        const messageDialog = findChild(signup, "messageDialog") as MessageDialog;

        verify(forenameField);
        verify(surnameField);
        verify(usernameField);
        verify(passwordField);
        verify(signUpBtn);
        verify(messageDialog);

        forenameField.text = "juan";
        surnameField.text = "gonzalez";
        usernameField.text = forenameField + surnameField;
        mouseClick(signUpBtn);

        verify(messageDialog.text === msgTemplate.arg("contraseña"));
    }

    function test_shouldReturnTrueIfUserSignedUp() {
        const forenameField = findChild(signup, "forenameField") as TextField;
        const surnameField = findChild(signup, "surnameField") as TextField;
        const usernameField = findChild(signup, "usernameField");
        const passwordField = findChild(signup, "passwordField");
        const signUpBtn = findChild(signup, "signUpBtn") as Button;
        const messageDialog = findChild(signup, "successMsg") as MessageDialog;

        verify(forenameField);
        verify(surnameField);
        verify(usernameField);
        verify(passwordField);
        verify(signUpBtn);
        verify(messageDialog);

        forenameField.text = "juan";
        surnameField.text = "gonzalez";
        usernameField.text = forenameField + surnameField;
        passwordField.text = "1234";
        mouseClick(signUpBtn);

        verify(messageDialog.text === "Te has registrado exitosamente");
    }
}
