import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import QtCharts
import Customer
import Utils
import "qrc:/utils"

ScrollablePage {
    id: root

    required property var customer
    readonly property T.StackView stackView: StackView.view

    title: "Detalles"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }
    scrollView.contentWidth: scrollView.availableWidth
    columnLayout.spacing: 20
    Component.onCompleted: {
        const handler = (data) => {
            const months = [];
            const orderQty = [];
            const currDate = new Date();
            const locale = Qt.locale();
            for (let element of data) {
                const month = element.month;
                currDate.setMonth(month-1);
                months.push(currDate.toLocaleString(locale, { month: 'long' }) + ` ${element.year}`);
                orderQty.push(element.order_qty);
            }
            barCategory.categories = months;
            barSeries.append("Cantidad de ordenes", [orderQty]);
        }
        fetchData();
    }

    CustomerInfoBox {
        id: infoBox
        customer.forename: model.forename
        customer.surname: model.surename
        customer.customerId: model.customerId
        Layout.fillWidth: true
    }

    ChartView {
        title: "Meses vs Cantidad de ordenes"
        legend.alignment: Qt.AlignBottom
        antialiasing: true
        Layout.fillWidth: true
        Layout.fillHeight: true

        BarSeries {
            id: barSeries
            axisX: BarCategoryAxis {
                id: barCategory
            }
        }
    }

    function fetchData(callback) {
        const base = "http://127.0.0.1:5000/api/statistics/customers/?start_year=2020&end_year=2024";
        const xhr = new XMLHttpRequest;
        xhr.onreadystatechange = () => {
            if (xhr.readyState === XMLHttpRequest.Done)
                if (callback)
                    callback(JSON.parse(xhr.responseText.toString()))
        }

        xhr.open("GET", base);
        xhr.send();
    }
}
