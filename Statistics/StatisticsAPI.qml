import QtQuick

import "qrc:/utils"

CaraApi {
    id: statisticsApi

    required property int salespersonId

    endpoint: "/statistics"

    signal mostSellingProductsFetched(products: var)
    signal customerStatisticsFetched(customers: var)

    function fetchMostSellingProducts() {
        get("/"+salespersonId+"/products");
        succeeded.connect(function(resp) {
            mostSellingProductsFetched(resp.most_selling_products);
        });
    }

    function fetchBillsStatistics(): void {
        get("/"+salespersonId+"/customers");
        succeeded.connect(function(resp) {
            customerStatisticsFetched(resp.customers_statistics);
        });
    }

    function fetchStatistics() {
        get("/");
        succeeded.connect(function(resp) {
            mostSellingProductsFetched(resp.statistics.most_selling_products);
            customerStatisticsFetched(resp.statistics.customers);
        });
    }
}
