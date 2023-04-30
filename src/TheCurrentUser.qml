pragma Singleton
import QtQuick
import Qt.labs.settings
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

    readonly property string orderFormPath: isAdmin ? "qrc:/orders/OrderFormAdmin.qml"
                                                    : "qrc:/cart/Cart.qml"
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
    property bool isVerificationDone: false

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
        base: Constants.base + "/users"
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
    signal verificationDone

    Component.onCompleted: {
        print("isGuest: "+isGuest);
        print("isAdmin: "+isAdmin);
        if (previousLogin) {
            print("previousLogin is " + previousLogin)
            TheCurrentUser.init();
        } else {
            print("No previous login");
            root.notLoggedIn();
        }
        isVerificationDone = true;
    }

    onIsAdminChanged: root.adminChanged()
    onHomePathChanged: root.homeChanged()
    onIsVerificationDoneChanged: if (isVerificationDone) root.verificationDone()

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
        isVerificationDone = true;
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
        isVerificationDone = true;
        loggedOut();
    }
}
