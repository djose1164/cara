import QtQuick
import com.cara
import "qrc:/utils"

CaraApi {
    id: statisticsApi
    endpoint: "/statistics"
    accessToken: CurrentUser.accessToken

    signal mostSellingProductsFetched(products: var)
    signal unpaidBillsFetched(customers: var)

    function fetchMostSellingProducts() {
        get("/");
        succeeded.connect(function(resp) {
            mostSellingProductsFetched(resp.statistics);
        });
    }

    function fetchStatistics() {
        get("/");
        succeeded.connect(function(resp) {
            mostSellingProductsFetched(resp.statistics.most_selling_products);
            unpaidBillsFetched(resp.statistics.customers);
        });
    }
}
