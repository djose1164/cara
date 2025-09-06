pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtNetwork
import QtCore
import Cara
import Cara.Data
import Cara.Authentication
import Cara.User
import Cara.Salesperson
import Cara.Controls
import Cara.ProductInventory
import Cara.Common
import Cara.Customer

ApplicationWindow {
    id: app
    width: Constants.appWidth
    height: Constants.appHeight
    visible: true
    title: "Cara"

    property bool authenticatedUser
    readonly property alias appStack: stackView
    readonly property url mainPage: "qrc:/qt/qml/Cara/MainPage.qml"

    onClosing: close => closingHandler(close)

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Component {
            Item {
                BusyIndicator { anchors.centerIn: parent }
            }
        }
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    RestService {
        HealthResource {
            id: healthResource
            path: "health"

            onServerHealthy: SessionManager.authenticateRefreshToken();

            onTimeout: timeoutDialog.open();

            onErrorOccurred: message => {
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

    SidePane {
        id: sideMenu
        implicitWidth: app.width
        implicitHeight: app.height
        stackView: appStack
    }

    Connections {
        target: SessionManager

        function onSessionStarted() {
            console.log("Session started!");
            UserSettings.setEntity(SessionManager.currentSession.user)
            console.log("UserSettings: ", UserSettings.entity());
            if (UserSettings.sellerId) {
                ProductInventoryController.updateSellerInventory(UserSettings.sellerId);
                CustomerManager.populate(UserSettings.sellerId)
            }
            app.appStack.replaceCurrentItem(app.mainPage);
        }

        function onSessionCleared() {
            app.appStack.replaceCurrentItem(app.mainPage);
        }

        function onAuthenticatedChanged() {
            console.log("SessionManager's authenticated changed!");
        }

        function onStoredSessionChecked(found: bool) {
            const isOnline = NetworkInformation.reachability === NetworkInformation.Reachability.Online;
            if (found && isOnline)
                healthResource.readStatus();
            else
                app.appStack.replaceCurrentItem(app.mainPage);
        }

        function onCurrentSessionChanged() {
            console.log("SessionManager's cuarrent session has changed!");
        }
    }

    function closingHandler(close: CloseEvent) {
        // Manages Android Back-button
        if (Qt.platform.os === "android") {
            if (appStack.depth > 1) {
                close.accepted = false;
                appStack.popCurrentItem();
                return;
            }

            const currentSV = appStack.currentItem.currentStackView;
            if (currentSV instanceof StackView && currentSV.depth > 1) {
                close.accepted = false;
                currentSV.popCurrentItem();
                return;
            }
        }
    }

    function clearSavedSettings() {
        UserSettings.clear();
        SalespersonSettings.clear();
    }
}
