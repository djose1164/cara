pragma Singleton
import QtQuick
import Customer

Item {
    readonly property alias model: model
    readonly property bool isEmpty: model.count === 0

    signal updateModel

    CustomerModel {
        id: model
        repository: customerRepository
    }

    RestCustomerRepository { id: customerRepository }

    Connections {
        target: model
        function onUpdated() {
            print("CustomerManager: model was populated. Count: "+model.count)
        }
    }

    function populate(salespersonId: int) {
        print("CustomerManager: populating.");
        model.update(salespersonId);
    }

    function addCustomer(customer) {
        model.append(customer);
    }

    function clearModel() {
        model.clear();
    }
}
