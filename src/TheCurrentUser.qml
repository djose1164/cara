pragma Singleton
import QtQuick
import QtCore

QtObject {
    id: root
    readonly property int userId: settings.userId
    property alias isLoggedIn: settings.isLoggedIn

    property alias forename: settings.forename
    property alias surname: settings.surname

    readonly property bool isAdmin: settings.userTypeId === TheCurrentUser.Types.Admin
    readonly property bool isCostumer: settings.customerId

    readonly property string homePath: isAdmin ? "qrc:/home/AdminHome.qml"
                                            : "qrc:/home/UserHome.qml"
    enum Types {
        Admin = 1,
        User
    }

    signal userIdReady(userId: int)

    property Settings settings: Settings {
        id: settings
        property int userId
        property int customerId
        property string username
        property string forename
        property string surname
        property int userTypeId
        property string email
        property bool isLoggedIn

        category: "Credentials"

        onUserIdChanged: userIdReady(userId)
    }


    function init(client) {
        print("init " + JSON.stringify(client, 2));
        if (client.person_info) {
            settings.forename = client.person_info.forename;
            settings.surname = client.person_info.surname;
        }
        settings.email = client.email;
        settings.userTypeId = client.user_type_id ? client.user_type_id : client.userTypeId;
        settings.userId = client.id ? client.id : client.userId;
        settings.customerId = client.customer_id ? client.customer_id
                                                 : (client.customerId ? client.customerId : 0)
    }

    function clear() {
        settings.forename = "";
        settings.surname = "";
        settings.email = "";
        settings.userTypeId = 0;
        settings.userId = 0;
        settings.isLoggedIn = false;
    }
}
