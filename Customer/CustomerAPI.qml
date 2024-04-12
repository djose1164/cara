import QtQuick
import QtQuick.Controls
import Utils


@Deprecated { reason: "Use SalespersonController." }
QtObject {
    id: root

    signal customerCreated(customer: var)
    signal customerUpdated
    signal customerFetched(customer: var)
    signal customersFetched(customers: var)

    property int salespersonId

    property CaraApi rest: CaraApi{
        endpoint: "/customers"
    }

    function fetchCustomers() {
        rest.get(`/info/?salesperson_id=${salespersonId}`);
        rest.succeeded.connect(resp => customersFetched(resp.customers));
    }

    function create(client) {
        rest.post("/", client);
        rest.succeeded.connect(resp => customerCreated(resp.customer));
    }

    function updateForename(forename: string) {
        rest.patch("/", {forename});
    }

    function updateSurname(surname: string) {
        rest.patch("/", {surname});
    }

    function updateCustomer(customerId: int, customer: var) {
        rest.succeeded.connect(customerUpdated);
        rest.put(customer);
    }

    function fetchCustomerById(customerId: int) {
        rest.succeeded.connect(function(resp) {
            customerFetched(resp.customer);
        });
        rest.get(`/info/${customerId}`);
    }
}
