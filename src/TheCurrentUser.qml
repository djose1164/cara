pragma Singleton
import QtQuick
import QtCore
import "qrc:/utils"
import com.cara

QtObject {
    id: root

    readonly property bool isAdmin: userTypeId === TheCurrentUser.Types.Admin
    readonly property bool isCostumer: customerId > 0
    readonly property bool previousLogin: settings.value("refreshToken")
    readonly property string homePath: isAdmin ? "qrc:/home/AdminHome.qml"
                                               : "qrc:/home/UserHome.qml"
    property alias accessToken: settings.accessToken
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
        property string accessToken
        property string refreshToken

        category: "Credentials"
    }

    property Rest request: Rest {
        id: request
        property Refresh refresh: Refresh {
            id: refresh
            onAccessTokenReceived: token => initOnRefreshToken("Bearer " + token)
            onFailed: r => failedToSetUserCredentials(r)
            refreshToken: settings.refreshToken
        }
        base: "http://127.0.0.1:5000/api/users"
        accessToken: settings.accessToken
        onSucceeded: r => initWithUser(r.user)

        function fetchUserInfo() {
            print("fetchUserInfo");
            refresh.refresh(settings.refreshToken);
        }
    }

    signal userCredentialsSet
    signal failedToSetUserCredentials(respose: var)

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
        settings.accessToken = "Bearer " + response.access_token;
        settings.refreshToken = "Bearer " + response.refresh_token;

        initWithUser(response.user);
    }

    function initOnRefreshToken(token) {
        settings.accessToken = token;

        request.get("/");
    }

    function initWithUser(user) {
        print("initWithUser");
        const person_info = user.person_info;
        if (person_info) {
            forename = person_info.forename;
            surname = person_info.surname;
        }
        email = user.email;
        userTypeId = user.user_type_id;
        userId = user.id;
        customerId = user.customer_id ? user.customer_id : 0;
        userCredentialsSet();
    }

    function clear() {
        forename = "";
        surname = "";
        email = "";
        userTypeId = 0;
        userId = 0;
        isLoggedIn = false;
        settings.accessToken = "";
        settings.refreshToken = "";
    }
}
