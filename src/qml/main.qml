import QtQuick
import QtQuick.Controls
import "qrc:/errors"
import "qrc:/singleton"
import "qrc:/utils"
import core
import data
import Authentication as Auth
import User
import Utils

ApplicationWindow {
    id: app
    width: Constants.appWidth
    height: Constants.appHeight
    visible: true
    title: "Cara"

    property string iconSize: "24x24"
    property bool authenticatedUser
    readonly property alias appStack: stackView
    readonly property string mainPage: "qrc:/main/MainPage.qml"

    onClosing: function(close) {
        //Manages Android Back-button
        if(Qt.platform.os === "android") {
            if (appStack.depth > 1) {
                close.accepted = false;
                appStack.pop();
            }
            else
                close.accepted = true;
        }
    }

    Component.onCompleted: {
        LoginManager.session = UserSession.session;
        internetStatus.checkInternetConnection();
    }

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
            const user = userController.create(
                JSON.stringify(JSON.parse(resp).user)
            );
            print("Replacing appStack with MainPage");
            stackView.replace(mainPage, {user});
        }
    }

    Connections {
        target: LoginManager

        function onAuthenticationDone() {
            stackView.replace(mainPage, {user: userController.create({})});
        }
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    InternetStatus {
        id: internetStatus
        onStatusChanged: {
            print("InternetStatus: "+statusString);
            if (status === internetStatus.InternetNotAvailable)
                userModel.append({});
            else
                caraApi.checkStatus();
        }
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
        userStore: LocalUserStore { id: localUserStore }
        onAuthenticated: {
            print("onLoggedIn: "+localUserStore.user.userId)
            userController.getUserById(localUserStore.user.userId);
        }
    }

    function loginSetup() {
        print("user has previoues login: "+login.hasPreviousLogin);
        if (login.hasPreviousLogin) {
            print("Previous login found.")
            print("Authentication refresh token.")
            login.authenticateRefreshToken();
        } else {
            stackView.replace(mainPage, {user: userController.create({})});
        }
    }
}
