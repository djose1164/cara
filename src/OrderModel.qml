import QtQuick
import QtQuick.Controls

Item {
    property alias model: orderModel

    ListModel {
        id: orderModel
    }
    
    /*WorkerScript {
        id: worker
        source: "qrc:/js/order_request.mjs"
    }*/
    
    function findByOrderId(orderId) {
        worker.sendMessage({"type": "findByOrderId", "model": orderModel, "orderId": orderId});
    }
}
