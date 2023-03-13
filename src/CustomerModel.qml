import QtQuick
import "qrc:/users"
import com.cara 1.0

QtObject {
    id: root

    property alias model: model
    property alias request: request

    signal filtering
    signal customerFetched(customer: var)

    Component.onCompleted: rest.refreshAccessToken()

    property Rest rest: Rest {
        id: request
        base: "http://127.0.0.1:5000/api/customers"
        refreshToken: TheCurrentUser.refreshToken
    }

    property ListModel listModel: ListModel { id: model }

    // For model
    function setById(customerId) {
        const handler = r => model.set(find(customer => customer.id === customerId), r);
        request.succeeded.connect(handler)
        request.get("/" + customerId);
        request.succeeded.disconnect(handler);
    }

    function updateCustomer(customerId) {
        print("updateCustomer: customerId " + customerId);
        const idx = indexOf(item => item.id === customerId);
        print(`idx ${idx}`);
        request.succeeded.connect(function(r) {
            const fetched = r.customer;
            set(idx, fetched);
        });
        request.get("/" + customerId);
    }

    function getClientByName(name) {
        return find(text => text.forename.indexOf(name) !== -1);
    }

    function findAll() {
        print("Fecthing all customers...");
        request.succeeded.connect(function(r) {
            model.append(r.customers)
        });
        request.get("/");
    }

    function findByCustomerId(customerId) {
        print(`setById ${customerId}`);
        request.succeeded.connect(function(r) {
            model.append(r.customer);
        });
        request.get(`/${customerId}`);
    }

    function findOrdersByCustomerId(customerId) {
        print(`setById ${customerId}`);
        request.succeeded.connect(function(r) {
            clearModel();
            model.append(r.customer.orders);
        });
        request.get(`/${customerId}`);
    }

    function add(customer) {
        print("Adding customer", customer);
        model.append(customer);
    }

    function set(idx, obj) {
        print("idx "+idx+" typeof(obj) " + obj);
        model.set(idx, obj);
    }

    function getAt(index) {
        return model.get(index);
    }

    function filter(msg) {
        filtering();
    }

    function clearModel() {
        if (model.count)
            model.clear();
    }

    function isModelEmpty() {
        return model.count === 0;
    }

    function find(func) {
        for (let i = 0; i < model.count; ++i)
            if (func(model.get(i)))
                return model.get(i);

        return null;
    }

    function likeName(text) {
        print("lineName: name " + text);
        let matches = [];
        const predicate = c => {
            const name = c.person_info.forename + c.person_info.surname;
            return name.toLowerCase().includes(text.toLowerCase()) ? c : null;
        };
        for (let i = 0; i < model.count; ++i) {
            const fetched = predicate(model.get(i));
            if (fetched) {
                matches.push(fetched);
                print(JSON.stringify(fetched, null, 2))
            }
        }
        return matches;
    }

    function indexOf(func) {
        for (let i = 0; i < model.count; ++i)
            if (func(model.get(i)))
                return i;

        return null;
    }
}
