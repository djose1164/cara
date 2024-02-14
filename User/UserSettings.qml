pragma Singleton

import QtCore
import User

Settings {
    id: root

    property string forename
    property string surname
    property string email
    property string username
    property int userId
    property int userTypeId
    property int customerId

    category: "User"

    function entity(): user {
        return {
            username,
            forename,
            surname,
            userId,
            userType: userTypeId,
            customerId
        }
    }

    function setEntity(newUser: user) {
        root.forename = newUser.forename;
        root.surname = newUser.surname;
        root.email = newUser.email;
        root.username = newUser.username;
        root.userId = newUser.userId;
        root.userTypeId = newUser.userType;
        root.customerId = newUser.customerId;
    }

    function clear() {
        root.forename = "";
        root.surname = "";
        root.username = "";
        root.email = "";
        root.userId = 0;
        root.userTypeId = 0;
    }
}
