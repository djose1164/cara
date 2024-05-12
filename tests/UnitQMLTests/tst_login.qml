import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtTest
import Cara.Authentication

Item {
    id: root

    readonly property int minumumUsernameLength: 4
    //property LoginPage loginPage

    width: 420
    height: 600

    LoginPage {
        id: loginPage
        width: root.width
        height: root.height

    }

    SignalSpy {
        id: spy
        target: loginPage
    }

    TestCase {
        id: testCase
        name: "Login"
        when: windowShown

        function cleanup() {
            const usernameField = loginPage.usernameField;
            const passwordField = loginPage.passwordField;
            usernameField.text = "";
            passwordField.text = "";
            spy.clear();
        }

        function test_shouldReturnTrueIfUsernameIsNotEmpty() {
            const usernameField = loginPage.usernameField;

            usernameField.text = "cara";

            verify(usernameField.text.length >= root.minumumUsernameLength);
        }

        function test_shouldLoginIfCredentialsAreCorrect() {
            const usernameField = loginPage.usernameField;
            const passwordField = loginPage.passwordField;
            const loginBtn = loginPage.loginBtn;
            const busyIndicator = findChild(loginPage, "busyIndicator");
            spy.signalName = "loggedIn";

            usernameField.text = "cara";
            passwordField.text = "1234";
            mouseClick(loginBtn);

            verify(spy.valid);
            verify(busyIndicator);
            tryCompare(spy, "count", 1);
            tryCompare(busyIndicator, "visible", false);
        }

        function test_shouldShowPopupIfCredentialsAreInvalid() {
            //wait(175); // If we don't wait the test will fail.
            const usernameField = loginPage.usernameField;
            const passwordField = loginPage.passwordField;
            const loginBtn = loginPage.loginBtn;
            const popup = findChild(loginPage, "popup") as MessageDialog;
            spy.signalName = "errorOccurred";

            usernameField.text = "cara";
            passwordField.text = "12345";
            mouseClick(loginBtn);

            verify(spy.valid);
            verify(popup);
            tryCompare(spy, "count", 1);
            compare(popup.text, "Usuario o contraseña incorrecta.");

            root.takeScreenShot("screenshots/test_shouldShowPopupIfCredentialsAreInvalid.png");
            popup.close();
        }

        function test_shouldShowToolTipIfUsernameIsEmpty() {
            const usernameField = loginPage.usernameField;
            const passwordField = loginPage.passwordField;
            const loginBtn = loginPage.loginBtn;
            const toolTip = findChild(usernameField, "usernameToolTip");

            passwordField.text = "1234";
            mouseClick(loginBtn);

            verify(toolTip)
            tryCompare(toolTip, "opacity", 1);

            root.takeScreenShot("screenshots/test_shouldShowToolTipIfUsernameIsEmpty.png");
            toolTip.hide();
        }

        function test_shouldShowToolTipIfPasswordIsEmpty() {
            //wait(175); // If we don't wait the test will fail.
            const usernameField = loginPage.usernameField;
            const passwordField = loginPage.passwordField;
            const loginBtn = loginPage.loginBtn;
            const toolTip = findChild(passwordField, "passwordToolTip");

            usernameField.text = "cara";
            loginBtn.clicked();

            verify(toolTip)
            tryCompare(toolTip, "opacity", 1);

            root.takeScreenShot("screenshots/test_shouldShowToolTipIfPasswordIsEmpty.png");
            toolTip.hide();
        }
    }

    function takeScreenShot(path: string) {
        const img = testCase.grabImage(loginPage);
        img.save(path);
    }
}
