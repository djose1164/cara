import QtQuick
import com.cara 1.0
import "qrc:/utils"

QtObject {
    id: root

    property CaraApi request: CaraApi {
        id: request
        accessToken: CurrentUser.accessToken
        endpoint: "/customers"

        Component.onCompleted: request.failed.connect(root.failed)
    }

    signal customerUpdated
    signal customerFetched(customer: var)
    signal failed(msg: string)

    function create(client) {
        print("Posting new client", JSON.stringify(client));
        request.post("/", client);
    }

    function updateForename(forename: string) {
        request.patch("/", {forename});
    }

    function updateSurname(surname: string) {
        request.patch("/", {surname});
    }

    function updateCustomer(customerId: int, customer: var) {
        request.succeeded.connect(customerUpdated);
        request.put(`/${customerId}`, customer);
    }

    function fetchCustomerById(customerId: int) {
        request.succeeded.connect(function(resp) {
            customerFetched(resp.customer);
        });
        request.get(`/info/${customerId}`);
    }
}
