import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

ListView {
    id: root

    signal modelPopulated

    delegate: BillDelegate {
        required property string forename
        required property string surname
        required property var bills
        required customerId

        name: forename + surname
        orders: bills
    }

    model: customerModel

    CustomerModel {
        id: customerModel

        onPopulated: modelPopulated();
    }

    function populateModel() {
        customerModel.populate();
    }
}
