import QtQuick
import QtQuick.Controls
import QtNetwork
import Cara
import Cara.Data
import Cara.Authentication as Auth
import Cara.User
import Cara.Salesperson
import Cara.Controls

ApplicationWindow {
    id: app
    width: Constants.appWidth
    height: Constants.appHeight
    visible: true
    title: "Cara"

    property string iconSize: "24x24"
    property bool authenticatedUser
    readonly property alias appStack: stackView
    readonly property string mainPage: "qrc:/qt/qml/Cara/MainPage.qml"

    onClosing: close => closingHandler(close)

    Component.onCompleted: setup()

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Component {
            Item {
                BusyIndicator { anchors.centerIn: parent }
            }
        }
    }

    UserController {
        id: userController
        onFetched: function(resp) {
            const salesperson = JSON.parse(resp).user.salesperson;
            const user = userController.create(resp);
            print("Replacing appStack with MainPage: "+JSON.stringify(user,null, 2));
            UserSettings.setEntity(user);
            SalespersonSettings.setEntity(spController.fromJsonObject(salesperson));
            stackView.replace(mainPage);
        }
        onUserNotFound: {
            console.log("User with userId: "+UserSettings.userId+" was not found");
            console.log("Deleting saved user info...");
            UserSettings.clear();
            SalespersonSettings.clear();
            LoginManager.logOut();
            stackView.replace(mainPage);
        }
    }

    SalespersonController {
        id: spController
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    RestService {
        HealthResource {
            id: healthResource
            path: "health"
            onStatusReady: resp => {
                const data = JSON.parse(resp);
                if (data.status !== "OK") {
                    const message = "Nuestros servidores se encuentran actualmente bajo "
                                    + 'mantenimiento. Puedes ver como va en <a href="https://twitter.com/carardoficial">Twiterr</a>.';
                    appStack.replace("qrc:/errors/ErrorPage.qml", {message});
                } else {
                    loginSetup();
                }
            }
            onTimeout: {
                print("Time out on health")
                timeoutDialog.open();
            }
            onConnectionRefused: {
                const message = "No se ha podido establecer una conexión con el servidor. "
                              + "Intenta de nuevo más tarde.";
                appStack.replace("qrc:/errors/ErrorPage.qml", {message});
            }
            onUpgradeRequired: {
                const message = "Necesitas actualizar Cara a la ultima version.";
                appStack.replace("qrc:/errors/ErrorPage.qml", {message});
            }
            onErrorOccurred: message => {
                if (NetworkInformation.reachability !== NetworkInformation.Reachability.Disconnected) {
                    stackView.replaceCurrentItem(app.mainPage);
                    console.log("No internet: NetworkInformation.Reachability.Disconnected");
                    return;
                }
                console.log("onErrorOccurred: "+message);
                appStack.replace("qrc:/errors/ErrorPage.qml", {message});
            }
        }
    }

    TimeoutDialog {
        id: timeoutDialog
        width: app.width * .9
        height: app.height/2
        anchors.centerIn: Overlay.overlay
        onAccepted: healthResource.readStatus()
    }

    // In charge of checking if there's a previous login, if so logs the user in.
    // This' not supposod to handle authentication from credentials, only from refresh token.
    Auth.Login {
        id: login
        authenticator: Auth.LocalUserAuthentication {
            id: localAuthenticator
            refreshToken: Auth.TokenManager.refreshToken
            onTokenExpired: {
                print("Token is expired");
                clearSavedSettings();
                stackView.replaceCurrentItem(mainPage);
            }
        }

        tokenManager: Auth.TokenManager
        store: Auth.TokenStore {}
        onAuthenticated: data => {
            print("Login: authenticated: ");
            //Auth.TokenManager.accessToken = JSON.parse(data).access_token;
            userController.getUserById(UserSettings.userId);
        }
    }

    function setup() {
        LoginManager.session = UserSession.session;
        healthResource.readStatus();
    }

    function loginSetup() {
        if (Auth.TokenManager.refreshToken && UserSettings.userId > 0) {
            login.authenticate();
        } else {
            clearSavedSettings();
            stackView.replaceCurrentItem(mainPage);
        }
    }

    function closingHandler(close: CloseEvent) {
        // Manages Android Back-button
        if (Qt.platform.os === "android") {
            if (appStack.depth > 1) {
                close.accepted = false;
                appStack.pop();
                return;
            }

            const currentSV = appStack.currentItem.currentStackView;
            if (currentSV instanceof StackView && currentSV.depth > 1) {
                close.accepted = false;
                currentSV.pop()
                return;
            }
        }
    }

    function clearSavedSettings() {
        UserSettings.clear();
        SalespersonSettings.clear();
    }
}
