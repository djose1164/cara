import QtQuick
import "qrc:/utils"

CaraApi {
    id: api
    endpoint: "/orders/buy"

    signal buyOrderCreated(buyOrder: var)
    signal buyOrdersFetched(buyOrders: var)

    function createBuyOrder(buyOrder) {
        succeeded.connect(function(resp) {
            buyOrderCreated(resp.buy_order);
        });
        post("/", buyOrder);
    }

    function fetchBuyOrders() {
        succeeded.connect(function(resp) {
            buyOrdersFetched(resp.orders);
        });
        get("/");
    }
}
