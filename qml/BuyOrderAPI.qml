import QtQuick
import core
import "qrc:/utils"
import "qrc:/singleton"

CaraApi {
    id: api
    endpoint: "/orders/buy"
    accessToken: TokenManager.accessToken

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
        get("/"+CurrentUser.user.userId);
    }
}
