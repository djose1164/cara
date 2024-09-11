import QtQuick
import Cara.Utils

@Deprecated { reason: "Use the C++ QML_ELEMENT type" }
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
