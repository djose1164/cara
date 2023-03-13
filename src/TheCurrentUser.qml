pragma Singleton
import QtQuick
import Qt.labs.settings
import "qrc:/utils"
import com.cara 1.0

QtObject {
    id: root

    readonly property bool isAdmin: userTypeId === TheCurrentUser.Types.Admin
    readonly property bool isCostumer: customerId > 0
    readonly property bool previousLogin: settings.value("refreshToken")
    readonly property string homePath: isAdmin ? "qrc:/home/AdminHome.qml"
                                               : "qrc:/home/UserHome.qml"
    readonly property string orderFormPath: isAdmin ? "qrc:/orders/OrderFormAdmin.qml"
                                                    : "qrc:/orders/OrderFormUser.qml"
    // TODO: This should be deleted
    property alias refreshToken: settings.refreshToken
    property int userId
    property int customerId
    property string username
    property string forename
    property string surname
    property int userTypeId
    property string email
    property bool isLoggedIn

    enum Types {
        Admin = 1,
        User
    }

    property Settings settings: Settings {
        id: settings
        property string refreshToken
        category: "Credentials"
    }

    property Rest request: Rest {
        id: request
        refreshToken: settings.refreshToken
        base: "http://127.0.0.1:5000/api/users"
        onFailed: r => failedToSetUserCredentials(r)
        onSucceeded: r => initWithUser(r.user)
        onAccessTokenChanged: request.get("/")
        onServiceUnavailable: root.serverUnavailable()

        function fetchUserInfo() {
            print("fetchUserInfo");
            refreshAccessToken();
        }
    }

    signal userCredentialsSet
    signal failedToSetUserCredentials(respose: var)
    signal orderPosted
    signal serverUnavailable

    Component.onCompleted: {
        if (previousLogin) {
            print("previousLogin is " + previousLogin)
            TheCurrentUser.init();
        }
    }

    function init(response=null) {
        if (response)
            initOnLogin(response);
        request.fetchUserInfo();
    }

    function initOnLogin(response) {
        settings.refreshToken = "Bearer " + response.refresh_token;

        initWithUser(response.user);
    }

    function initWithUser(user) {
        print("initWithUser");
        const person_info = user.person_info;
        if (person_info) {
            forename = person_info.forename;
            surname = person_info.surname;
            customerId = person_info.customer_id ? person_info.customer_id : 0;
        }
        email = user.email;
        userTypeId = user.user_type_id;
        userId = user.id;
        print("user initialized")
        userCredentialsSet();
    }

    function clear() {
        forename = "";
        surname = "";
        email = "";
        userTypeId = 0;
        userId = 0;
        isLoggedIn = false;
        settings.refreshToken = "";
    }
}
