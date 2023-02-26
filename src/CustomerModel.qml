import QtQuick
import "qrc:/users"
import com.cara

Item {
    id: root

    property alias model: model
    property alias request: request

    signal filtering()

    Rest {
        id: request
        base: "http://127.0.0.1:5000/api/customers"
        accessToken: TheCurrentUser.accessToken
        onSucceeded: r => {
            print("Rest: onSucceeded: ");
            model.append(r.customers);
        }
    }

    ListModel { id: model }

    function setById(clientId) {
        get(r => {
            print(JSON.stringify(r, 2));
            model.set(0, r.customer);
        });

    }

    function addById(clientId) {
        get(r => {model.append(r.customer); print(JSON.stringify(r, 2))});
    }

    function get(func) {
        request.get("/" + clientId, null, func, TheCurrentUser.accessToken);
    }

    function getClientByName(name) {
        return find(text => text.forename.indexOf(name) !== -1);
    }

    function findAll() {
        print("Fecthing all customers...");
        request.get("/");
    }

    function add(client) {
        model.append(client);
    }

    function getAt(index) {
        return model.get(index);
    }

    function filter(msg) {
        filtering();
        send(msg);
    }

    function clearModel() {
        model.clear();
    }

    function isModelEmpty() {
        return model.count === 0;
    }
}
