import QtQuick
import QtQuick.Controls
import "qrc:/errors"
import data
import Authentication as Auth
import User
import Utils
import Salesperson

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
            print("Replacing appStack with MainPage");
            UserSettings.setEntity(user);
            SalespersonSettings.setEntity(spController.fromJsonObject(salesperson));
            stackView.replace(mainPage, {user});
        }
    }

    SalespersonController {
        id: spController
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    CaraApi {
        id: caraApi

        endpoint: "/health"
        //  FIXME: Health should return when the service is unavailable
        onServiceUnavailable: {
            const message = "Nuestros servidores se encuentran actualmente bajo "
                          + "mantenimiento. Puedes ver como va en [Twiterr](https://twitter.com/carardoficial).";
            appStack.replace("qrc:/errors/ErrorPage.qml", {message});
        }
        onSucceeded: {
            loginSetup();
        }
        Component.onCompleted: print("CaraApi: Healths", caraApi.base)

        function checkStatus() {
            get("/");
        }
    }

    // In charge of checking if there's a previous login, if so logs the user in.
    // This' not supposod to handle authentication from credentials, only from refresh token.
    Auth.Login {
        id: login
        authenticator: localAuthenticator
        stores: [
            Auth.TokenStore {
                id: myTokenStore
                onSaved: {
                    const userId = UserSettings.userId;
                    if (userId === 0) {
                        stackView.replace(mainPage, {user: userController.create({})});
                        return;
                    }

                    userController.getUserById(userId);
                }
            }
        ]
    }

    Auth.LocalUserAuthentication {
        id: localAuthenticator
        refreshToken: myTokenStore.refresh
        onTokenExpired: {
            print("Token is expired");
            stackView.replace(mainPage, {user: userController.create({})});
        }
    }

    function setup() {
        LoginManager.session = UserSession.session;
        myTokenStore.restore();
        caraApi.checkStatus();
    }

    function loginSetup() {
        if (myTokenStore.refresh) {
            print("Previous login found.")
            print("Authentication refresh token.")
            login.authenticate();
        } else {
            stackView.replace(mainPage, {user: userController.create({})});
        }
    }

    function closingHandler(close: CloseEvent) {
        // Manages Android Back-button
        if(Qt.platform.os === "android") {
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
}
