#include "ordertemplate.h"

#include <QFile>
#include <QSettings>
#include <QJsonArray>
#include <QTextDocument>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto htmlPath{":/qt/qml/Cara/Billing/order-model.html"_L1};
constexpr auto stylePath{":/qt/qml/Cara/Billing/style.css"_L1};
constexpr auto prefix{"/factura-%1.pdf"_L1};
constexpr auto td{"<td>%1</td>"_L1};
constexpr double disccount{};
}

namespace Billing
{
OrderTemplate::OrderTemplate(QObject *parent)
    : Billing::AbstractDocument{parent}
    , html_{}
    , style_{}
    , order_{}
    , calculatedTotal_{}
{
}

void OrderTemplate::loadHtml()
{
    html_ = load(htmlPath).value_or(QByteArray{});
}

void OrderTemplate::loadStyle()
{
    style_ = load(stylePath).value_or(QByteArray{});
}

QString OrderTemplate::getFormattedOrder()
{
    if (order_.isEmpty())
    {
        qCritical() << "OrderTemplate::getFormattedOrder: order cannot be empty.";
        emit errorOccurred("OrderTemplate::getFormattedOrder: order cannot be empty."_L1);
        return QString{};
    }

    if (!order_["order_details"].isArray())
    {
        qCritical() << "OrderTemplate::getFormattedOrder: order_details is not an array.";
        emit errorOccurred("OrderTemplate::getFormattedOrder: order_details is not an array."_L1);
        return QString{};
    }

    const auto detailsHtmlOpt{htmlDetails()};
    if (!detailsHtmlOpt)
    {
        qCritical() << "OrderTemplate::getFormattedOrder: empty HTML details.";
        emit errorOccurred("OrderTemplate::getFormattedOrder: empty HTML details."_L1);
        return QString{};
    }

    const auto detailsHtml{detailsHtmlOpt.value()};
    const auto createdAtString{order_["created_at"_L1].toString()};
    auto createdAt{QDateTime::fromString(createdAtString, Qt::ISODate)};
    createdAt.setTimeZone(QTimeZone{QTimeZone::UTC});
    Q_ASSERT(calculatedTotal_);

    QSettings settings{};
    return html_.arg(
        settings.value("Salesperson/warehouseAddress"_L1).toString(),
        createdAt.toLocalTime().toString("dd/MM/yyyy hh:mm:ss"_L1),
        order_["customer"_L1]["name"_L1].toString(),
        QString::number(order_["id"].toInt()),
        detailsHtml.join(""),
        QString::number(calculatedTotal_),
        QString::number(disccount),
        QString::number(calculatedTotal_)
    );
}

std::optional<QByteArray> OrderTemplate::load(QAnyStringView filename)
{
    QFile f{filename.toString()};
    if (!f.open(QFile::ReadOnly))
    {
        const QString errorStr{"Failed to open %1: %2"_L1
                               .arg(stylePath, f.errorString())};
        qCritical() << "PdfOrderPrinter::load: " << errorStr;
        emit errorOccurred(errorStr);
        return std::nullopt;
    }

    return f.readAll();
}

void OrderTemplate::setup()
{
    loadHtml();
    loadStyle();
}

std::optional<QStringList> OrderTemplate::htmlDetails()
{
    resetCalculatedTotal();
    const auto orderDetails = order_["order_details"].toArray();
    QStringList detailsHtml{};
    for (const auto &detail : std::as_const(orderDetails))
    {
        qDebug() << "OrderTemplate::htmlDetails: " << detail;
        if (!detail.isObject())
        {
            constexpr auto errorStr{"detail is not an object."_L1};
            qCritical() << "OrderTemplate::htmlDetails: " << errorStr;
            qCritical() << detail;
            emit errorOccurred(errorStr);
            return std::nullopt;
        }

        const auto detailObject{detail.toObject()};
        if (!detailObject["product"].isObject())
        {
            constexpr auto errorStr{"product is not an object."_L1};
            qCritical() << "OrderTemplate::htmlDetails: " << errorStr;
            emit errorOccurred(errorStr);
            return std::nullopt;
        }

        Q_ASSERT(detailObject.contains("price"_L1));

        bool ok{};
        const auto productObject{detailObject["product"_L1].toObject()};
        const double sellPrice{detailObject["price"_L1].toString().toDouble(&ok)};
        Q_ASSERT(ok);
        const int quantity{detailObject["quantity"_L1].toInt()};
        const auto productName{productObject["name"_L1].toString()};
        const auto subtotal{quantity * sellPrice};

        Q_ASSERT(quantity);
        Q_ASSERT(sellPrice > 0);
        calculatedTotal_ += subtotal;

        detailsHtml
            << "<tr>"_L1
            << td.arg(QString::number(quantity) + "x"_L1 + QString::number(sellPrice))
            << td.arg(productName)
            << td.arg(QString::number(subtotal))
            << "</tr>"_L1;
    }

    return detailsHtml;
}

void OrderTemplate::resetCalculatedTotal()
{
    calculatedTotal_ = 0;
}

QString OrderTemplate::style() const
{
    return style_;
}

void OrderTemplate::print(QPagedPaintDevice *printer)
{
    QTextDocument doc{};
    doc.setPageSize(pageSize());
    doc.setDefaultStyleSheet(style_);
    doc.setHtml(getFormattedOrder());
    doc.print(printer);
}

QJsonObject OrderTemplate::order() const
{
    return order_;
}

void OrderTemplate::setOrder(const QJsonObject &newOrder)
{
    if (order_ == newOrder)
        return;
    order_ = newOrder;
    emit orderChanged();
}

void OrderTemplate::classBegin()
{
}

void OrderTemplate::componentComplete()
{
    setup();
}
}
