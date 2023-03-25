pragma Singleton
import QtQuick
import Qt.labs.settings
import "qrc:/utils"
import com.cara 1.0

QtObject {
    id: root

    property int userType: previousLogin ? (isAdmin ? TheCurrentUser.Types.Admin
                                                    : TheCurrentUser.Types.User)
                                         : TheCurrentUser.Types.Guest
    readonly property bool isAdmin: userTypeId === TheCurrentUser.Types.Admin
    // To be customer, the user must have at least one order made.
    readonly property bool isCostumer: customerId > 0
    readonly property bool isGuest: userType === TheCurrentUser.Types.Guest
    readonly property bool previousLogin: settings.value("refreshToken")
    readonly property string homePath: isAdmin ? "qrc:/home/AdminHome.qml"
                                               : "qrc:/home/UserHome.qml"
    readonly property bool isUser: userTypeId === TheCurrentUser.Types.User

    readonly property string orderPath: isGuest ? "qrc:/orders/OrderFormUser.qml"
                                                : "qrc:/orders/OrderFormAdmin.qml"
    readonly property string orderFormPath: isAdmin ? "qrc:/orders/OrderFormAdmin.qml"
                                                    : "qrc:/orders/OrderFormUser.qml"
    // TODO: This should be deleted
    property alias refreshToken: settings.refreshToken
    readonly property alias accessToken: request.accessToken
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
        User,
        Guest
    }

    property Settings settings: Settings {
        id: settings
        property string refreshToken
        category: "Credentials"
    }

    property Rest request: Rest {
        id: request
        refreshToken: settings.refreshToken
        base: "https://cara-rd.live/api/users"
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
    signal loggedOut
    signal adminChanged
    signal homeChanged
    signal notLoggedIn

    Component.onCompleted: {
        print("isGuest: "+isGuest);
        print("isAdmin: "+isAdmin);
        if (previousLogin) {
            print("previousLogin is " + previousLogin)
            TheCurrentUser.init();
        } else {
            root.notLoggedIn();
        }
    }

    onIsAdminChanged: root.adminChanged()
    onHomePathChanged: root.homeChanged()

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
        userType = userTypeId;
        print("user initialized: userTypeId: "+userTypeId+"\nuserType: "+userType)
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
        userType = TheCurrentUser.Types.Guest;
        customerId = 0;
        loggedOut();
    }
}
