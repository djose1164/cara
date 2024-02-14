pragma Singleton
import QtQuick
import Customer
import Utils

Item {
    id: root

    readonly property alias model: model
    readonly property bool isEmpty: model.count === 0

    signal populating

    CustomerModel {
        id: model
        onUpdated: root.state = "updated"
    }

    RestService {
        CustomerResource {
            id: customerRepository
            path: "customers"
            onFetched: resp => model.set(JSON.parse(resp).customers)
            onContentNotFound: root.state = "contentNotFound"
        }
    }

    function populate(salespersonId: int) {
        print("CustomerManager: populating.");
        root.populating();
        root.state = "fetching";
        customerRepository.readBySalespersonId(salespersonId);
    }

    function addCustomer(customer) {
        model.append(customer);
    }

    function clearModel() {
        model.clear();
    }
}
