import QtCore
import QtQuick

QtObject {
    readonly property string forename: _.fromContact("forename")
    readonly property string surname: _.fromContact("surname")
    readonly property string email: _.fromContact("email")

    readonly property string username: _.fromCredentials("username")
    readonly property string accessToken: _.fromCredentials("accessToken")
    readonly property int userId: _.fromCredentials("userId")
    readonly property int userTypeId: _.fromCredentials("userTypeId")

    readonly property QtObject _: QtObject {
        readonly property Settings settings: Settings {}

        function fromCredentials(key: string): string {
            return settings.value("Credentials/"+key);
        }

        function fromContact(key: string): string {
            return settings.value("Contact/"+key);
        }
    }
}
