import QtQuick
import com.cara 1.0
import "qrc:/users"

QtObject {
    id: root

    property Rest request: Rest {
        id: request
        base: "https://cara-rd.live/api/customers"
        refreshToken: TheCurrentUser.refreshToken

    }

    function create(client) {
        print("Posting new client", JSON.stringify(client));
        request.accessTokenChanged.connect(function() {
            request.post("/", client);
        });
        request.refreshAccessToken();
    }
}
