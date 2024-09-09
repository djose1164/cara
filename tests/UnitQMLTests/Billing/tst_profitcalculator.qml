import QtQuick
import QtTest
import Cara.Salesperson

TestCase {
    name: "ProfitCalculator"
    width: 400
    height: 620

    property ProfitCalculatorPage profitCalculator

    Component {
        id: component
        ProfitCalculatorPage {
            width: 400
            height: 620
        }
    }

    function init() {
        profitCalculator = createTemporaryObject(component, parent);
    }

    function test_shouldDisableProductSelectionIfSupplierIsNotSelected() {
        verify(false);
    }
}
