import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0

ListView {
    id: root

    property StackView stackView

    signal modelPopulated

    delegate: BillDelegate {
        required property string forename
        required property string surname
        required bills
        required customerId

        name: forename + surname
    }

    model: customerModel

    CustomerModel {
        id: customerModel

        onPopulated: modelPopulated();
    }

    function populateModel() {
        customerModel.populate();
    }

    function clearModel() {
        customerModel.clear();
    }
}
