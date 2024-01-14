import QtQuick
import "qrc:/utils"
import "qrc:/singleton"

CaraApi {
    id: api

    required property int salespersonId

    signal buyOrderCreated(resp: var)
    signal buyOrdersFetched(buyOrders: var)

    endpoint: "/salespersons/" + salespersonId

    function createBuyOrder(buyOrder) {
        succeeded.connect(function(resp) {
            buyOrderCreated(resp);
        });
        post("/buy_orders", buyOrder);
    }

    function fetchBuyOrders(): void {
        succeeded.connect(function(resp) {
            buyOrdersFetched(resp.buy_orders);
        });
        get("/buy_orders");
    }
}
