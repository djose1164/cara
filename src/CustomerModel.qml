import QtQuick
import "qrc:/singleton"
import "qrc:/utils"

// TODO: The API must be change to use fetch instead of find to get resources
// from server

QtObject {
    id: root

    property alias model: model
    property alias request: request

    signal filtering
    signal customerFetched(customer: var)

    property CaraApi rest: CaraApi {
        id: request
        endpoint: "/customers"
        accessToken: TheCurrentUser.accessToken
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
        const idx = indexOf(item => item.id === customerId);
        request.succeeded.connect(function(r) {
            const fetched = r.customer;
            set(idx, fetched);
        });
        request.get("/" + customerId);
    }

    function setCustomerPayment({ customerId, payment }) {
        let orders = findOrderByCustomerId(customerId);
        if (!orders)
            return;
        const pred = order => order.payment.id === payment.id;
        let order = findInModel(pred, orders);
        order.payment = payment;
    }

    function getClientByName(name) {
        return find(text => text.forename.indexOf(name) !== -1);
    }

    function findAll() {
        request.succeeded.connect(function(r) {
            model.append(r.customers)
        });
        request.get("/");
    }

    function findByCustomerId(customerId) {
        request.succeeded.connect(function(r) {
            model.append(r.customer);
        });
        request.get(`/${customerId}`);
    }

    function findOrdersByCustomerId(customerId) {
        request.succeeded.connect(function(r) {
            clearModel();
            model.append(r.customer.orders);
        });
        request.get(`/${customerId}`);
    }

    function findOrderByCustomerId(customerId) {
        return find(customer => customer.id === customerId).orders;
    }

    function add(customer) {
        model.append(customer);
    }

    function set(idx, obj) {
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

    // TODO: This function should be added to a component that inherits from
    // ListModel
    function findInModel(predicate, model) {
        for (let i = 0; i < model.count; ++i)
            if (predicate(model.get(i)))
                return model.get(i);

        return null;
    }

    function likeName(text) {
        let matches = [];
        const predicate = c => {
            const name = c.person_info.name;
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
