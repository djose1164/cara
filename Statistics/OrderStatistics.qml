import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import QtCharts
import "qrc:/utils"

Page {
    id: root

    required property int salespersonId
    readonly property T.StackView stackView: StackView.view

    title: "Estadisticas de Clientes"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    Component.onCompleted: {
        const customersHandler = (data) => {
            const months = [];
            const orderQty = [];
            const currDate = new Date();
            const locale = Qt.locale();
            for (let element of data.month_vs_order_qty) {
                const month = element.month;
                currDate.setMonth(month-1);
                const monthString = currDate.toLocaleString(locale, "MMMM") + ` ${element.year}`;
                months.push(monthString);
                orderQty.push(element.order_qty);
            }
            barCategory.categories = months;

            barSeries.append("Cantidad de ordenes", orderQty);
            valueAxis.max = orderQty.reduce((a, b) => Math.max(a, b), -Infinity);
        }

        fetchData("/orders/?start_year=2020&end_year=2024", customersHandler);

        const paymentsHandler = data => {
            const stats = {Credito: 0, Abonado: 0, Pago: 0};
            for (let el of data.statistics) {
                stats[el.status] += el.qty;
            }
            for (let key of Object.keys(stats))
                pieSeries.append(key, stats[key]);
        }

        fetchData("/payment_status/?start_year=2020&end_year=2024", paymentsHandler);
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        ColumnLayout {
            width: root.width
            ChartView {
                id: chartView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: root.height * .8
                title: "Meses vs Cantidad de ordenes"
                legend.alignment: Qt.AlignBottom
                antialiasing: true

                BarSeries {
                    id: barSeries
                    axisX: BarCategoryAxis {
                        id: barCategory
                    }

                    axisY: ValuesAxis { id: valueAxis }
                }

                PinchHandler {
                    property double prevScale: 1.0
                    target: null
                    onScaleChanged: delta => {
                        const newScale =  delta;
                        if (newScale > prevScale)
                        print("Zoom in");
                        else
                        print("Zoom out");
                        chartView.zoom(newScale);
                        prevScale = newScale;
                    }
                }

                DragHandler {
                    target: null
                    onGrabChanged: function(_, point) {
                        const lastPosition = point.lastPosition;
                        const position = point.position;
                        const dX = (lastPosition.x/4 + position.x/4)/4;
                        if (lastPosition === position)
                            print("Do nothing");
                        else if (position.x > lastPosition.x)
                            chartView.scrollLeft(dX);
                        else if (position.x < lastPosition.x)
                            chartView.scrollRight(dX);
                        print("last position: "+point.lastPosition);
                        print("position: "+point.position);
                    }
                }
            }

            ChartView {
                title: "Estado de pago de las ordenes"
                legend.alignment: Qt.AlignBottom
                antialiasing: true
                Layout.preferredHeight: root.height * .8
                Layout.fillWidth: true
                Layout.fillHeight: true

                PieSeries {
                    id: pieSeries
                }
            }
        } // ColumnLayout
    } // ScrollView

    function fetchData(endpoint, callback) {
        const base = "http://127.0.0.1:5000/api/statistics"+endpoint;
        const xhr = new XMLHttpRequest;
        xhr.onreadystatechange = () => {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                const response = xhr.responseText.toString();
                print("response type: "+xhr.responseType);
                callback(JSON.parse(response))
            }
        }

        xhr.open("GET", base);
        xhr.send();
    }
}
