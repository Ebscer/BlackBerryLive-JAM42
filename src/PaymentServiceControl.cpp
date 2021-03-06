
#include "PaymentServiceControl.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/Window>
#include <bb/platform/CancelSubscriptionReply>
#include <bb/platform/ExistingPurchasesReply>
#include <bb/platform/PriceReply>
#include <bb/platform/PurchaseReceipt>
#include <bb/platform/PurchaseReply>
#include <bb/platform/SubscriptionStatusReply>
#include <bb/platform/SubscriptionTermsReply>

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QString>

using namespace bb::platform;

static QString receiptToString(bb::platform::PurchaseReceipt r)
{
    const QString initialPeriod = QString::number(r.initialPeriod());
    const QDateTime startDate = r.startDate();
    const QString startDateStr = startDate.isNull() ? "N/A" : startDate.toString();
    const QDateTime endDate = r.endDate();
    const QString endDateStr = endDate.isNull() ? "N/A" : endDate.toString();
    const QString isSubscr = r.isSubscription() ? "true" : "false";
    const QString itemStateStr = QString::number(static_cast<int>(r.state()));

    const QString displayString = "Date: " + r.date().toString() +
        "\nID/SKU: " + r.digitalGoodId() + "/" + r.digitalGoodSku() +
        "\nPurchaseID/licenseKey: " + r.purchaseId() + "/" + r.licenseKey() +
        "\nMetadata: " + r.purchaseMetadata() +
        "\nItemState/isSubscription?: " + itemStateStr + "/" + isSubscr +
        "\nStart/End: " + startDateStr + "/" + endDateStr +
        "\nInitialPeriod: " + initialPeriod + "\n";

    return displayString;
}

PaymentServiceControl::PaymentServiceControl(QObject *parent)
    : QObject(parent)
    , m_paymentManager(new PaymentManager(this))
{
    // Get the window group ID and pass it to the PaymentService instance.
    const QString windowGroupId = bb::cascades::Application::instance()->mainWindow()->groupId();
    m_paymentManager->setWindowGroupId(windowGroupId);
}

PaymentServiceControl::~PaymentServiceControl()
{
}

/**
 * Request the purchase from the payment service based on the item's id, sku, name and metadata.
 */
void PaymentServiceControl::purchase(const QString &id, const QString &sku, const QString &name, const QString &metadata)
{
    if (id.isEmpty())
    {
        return;
    }

    qDebug() << "\nRequesting purchase. ID:" << id << "SKU:" << sku << "Name:" << name << "Metadata:" << metadata;

    const PurchaseReply *reply = m_paymentManager->requestPurchase(id, sku, name, metadata);
    connect(reply, SIGNAL(finished()), SLOT(purchaseResponse()));
}

/**
 * Invoked in response to the purchase signal. It differentiates between successful and
 * error responses and emits appropriate signal for each.
 */
void PaymentServiceControl::purchaseResponse()
{
    bb::platform::PurchaseReply *reply = qobject_cast<bb::platform::PurchaseReply*>(sender());
    Q_ASSERT(reply);

    if (reply->isError())	//emits error signal upon errors.
    {
        qDebug() << "Purchase response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
        emit infoResponseError(reply->errorCode(), reply->errorText());
    }
    else	//emits success signal upon success
    {
        const QString displayString = receiptToString(reply->receipt());
        qDebug() << "Purchase response success. " << displayString;

        emit purchaseResponseSuccess(displayString);
    }

    reply->deleteLater();
}

/**
 * Request existing purchases from the payment service.
 */
void PaymentServiceControl::getExisting()
{
    qDebug() << "Get existing.";

    const ExistingPurchasesReply *reply = m_paymentManager->requestExistingPurchases(true);//true to refresh
    connect(reply, SIGNAL(finished()), SLOT(existingPurchasesResponse()));
}

/**
 * Invoked in response to retrieve existing purchases made and emit appropriate signal based
 * on the response data.
 */
void PaymentServiceControl::existingPurchasesResponse()
{
    bb::platform::ExistingPurchasesReply *reply = qobject_cast<bb::platform::ExistingPurchasesReply*>(sender());
    Q_ASSERT(reply);

    //emits error signal upon errors.
    if (reply->isError())
    {
        qDebug() << "Existing purchases response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
        emit infoResponseError(reply->errorCode(), reply->errorText());

    //emits success signal upon success
    }
    else
    {
        qDebug() << "Existing purchases response success. (TODO)";
        const QList<PurchaseReceipt> receipts = reply->purchases();

        if (receipts.isEmpty())
        {
            qDebug() << "Existing purchases response success. (No purchases)";
            //emit existingPurchasesResponseSuccess("(No purchases)");
        }
        else
        {
            //QString displayString;
            Q_FOREACH(PurchaseReceipt r, receipts)
            {
            	emit existingPurchasesResponseSuccess(r.digitalGoodId());
                //displayString += (receiptToString(r) + "\n");
            }
            //emit existingPurchasesResponseSuccess(displayString);
        }
    }

    reply->deleteLater();
}

/**
 * Query the payment service for an item's price based on its ID and SKU.
 */
void PaymentServiceControl::getPrice(const QString &id, const QString &sku)
{
    if (id.isEmpty())
    {
        return;
    }

    qDebug() << "Requesting price. ID: " << id << " SKU: " << sku;

    //Make the price request and indicate what method to invoke on signal response.
    const PriceReply *reply = m_paymentManager->requestPrice(id, sku);
    connect(reply, SIGNAL(finished()), SLOT(priceResponse()));
}

/**
 * Invoked in response to price request for an item. Emit appropriate signal based on response data.
 */
void PaymentServiceControl::priceResponse()
{
    bb::platform::PriceReply *reply = qobject_cast<bb::platform::PriceReply*>(sender());
    Q_ASSERT(reply);

    //emits error signal upon errors.
    if (reply->isError())
    {
        qDebug() << "Price response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
        emit infoResponseError(reply->errorCode(), reply->errorText());

    //emits success signal upon success
    }
    else
    {
        qDebug() << "Price response success. Price: " << reply->price();
        //Emit success response if no errors occurred.
        emit priceResponseSuccess(reply->price());
    }

    reply->deleteLater();
}

/**
 * Query the payment service for items subscription terms based on its ID, and SKU.
 */
void PaymentServiceControl::getSubscriptionTerms(const QString &id, const QString &sku)
{
    if (id.isEmpty())
    {
        return;
    }

    qDebug() << "Requesting subscription terms. ID: " << id << " SKU: " << sku;

    const SubscriptionTermsReply *reply = m_paymentManager->requestSubscriptionTerms(id, sku);
    connect(reply, SIGNAL(finished()), SLOT(subscriptionTermsResponse()));
}

/**
 * Invoked based on items subscription terms request.
 */
void PaymentServiceControl::subscriptionTermsResponse()
{
    bb::platform::SubscriptionTermsReply *reply = qobject_cast<bb::platform::SubscriptionTermsReply*>(sender());
    Q_ASSERT(reply);

    //emits error signal upon errors.
    if (reply->isError())
    {
        qDebug() << "Sub terms response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
        emit infoResponseError(reply->errorCode(), reply->errorText());

    //emits success signal upon success
    }
    else
    {
        qDebug() << "Sub terms response success. Price: " << reply->price() <<
            "\nInitialPeriod: " << reply->initialPeriod() <<
            "\nRenewalPrice: " << reply->renewalPrice() <<
            "\nRenewalPeriod: " << reply->renewalPeriod();

        emit subscriptionTermsResponseSuccess(reply->price(), reply->initialPeriod(), reply->renewalPrice(), reply->renewalPeriod());
    }

    reply->deleteLater();
}

/**
 * Query the payment service for an item's subscription status based on its ID and SKU.
 */
void PaymentServiceControl::checkSubscriptionStatus(const QString &id, const QString &sku)
{
    if (id.isEmpty())
    {
        return;
    }

    qDebug() << "Check subscription status. ID: " << id << " SKU: " << sku;

    const SubscriptionStatusReply *reply = m_paymentManager->requestSubscriptionStatus(id, sku);
    connect(reply, SIGNAL(finished()), SLOT(subscriptionStatusResponse()));
}

/**
 * Invoked upon response from the subscription status query.
 */
void PaymentServiceControl::subscriptionStatusResponse()
{
    bb::platform::SubscriptionStatusReply *reply = qobject_cast<bb::platform::SubscriptionStatusReply*>(sender());
    Q_ASSERT(reply);

    //emits error signal upon errors.
    if (reply->isError())
    {
        qDebug() << "Check status response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
        emit infoResponseError(reply->errorCode(), reply->errorText());
    }
    else //emits success signal upon success
    {
        qDebug() << "Check status response success. Active? " << reply->isActive();
        emit checkStatusResponseSuccess(reply->isActive());
    }

    reply->deleteLater();
}

/**
 * Cancel item's subscription based on the purchase ID of that item.
 */
void PaymentServiceControl::cancelSubscription(const QString &purchaseId)
{
    if (purchaseId.isEmpty())
    {
        return;
    }

    qDebug() << "Cancel subscription. Purchase ID: " << purchaseId;

    const CancelSubscriptionReply *reply = m_paymentManager->requestCancelSubscription(purchaseId);
    connect(reply, SIGNAL(finished()), SLOT(cancelSubscriptionResponse()));
}

/**
 * Invoked in response to the subscription cancellation of a purchased item.
 */
void PaymentServiceControl::cancelSubscriptionResponse()
{
    bb::platform::CancelSubscriptionReply *reply = qobject_cast<bb::platform::CancelSubscriptionReply*>(sender());
    Q_ASSERT(reply);

    //emits error signal upon errors.
    if (reply->isError())
    {
        qDebug() << "Cancel subscription response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
        emit infoResponseError(reply->errorCode(), reply->errorText());
    }
    else //emits success signal upon success
    {
        qDebug() << "Cancel subscription response success. Canceled? " << reply->isCanceled();
        emit cancelSubscriptionResponseSuccess(reply->isCanceled());
    }

    reply->deleteLater();
}
