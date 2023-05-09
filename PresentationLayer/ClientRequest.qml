import QtQuick
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

QtObject {
    id: root

    property CaraApi request: CaraApi {
        id: request
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
