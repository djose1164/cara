import QtQuick
import com.cara 1.0
import "qrc:/users"

QtObject {
    id: root

    property CaraApi request: CaraApi {
        id: request
        endpoint: "/customers"
        accessToken: TheCurrentUser.accessToken
    }

    function create(client) {
        print("Posting new client", JSON.stringify(client));
        request.accessTokenChanged.connect(function() {
            request.post("/", client);
        });
        request.refreshAccessToken();
    }
}
