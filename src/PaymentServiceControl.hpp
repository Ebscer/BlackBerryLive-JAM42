
#ifndef PAYMENTSERVICECONTROL_HPP
#define PAYMENTSERVICECONTROL_HPP

#include <bb/platform/PaymentManager>

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

class PaymentServiceControl : public QObject
{
    Q_OBJECT
public:
    PaymentServiceControl(QObject *parent = 0);
    virtual ~PaymentServiceControl();

    //invokable purchase method from the qml control
    Q_INVOKABLE void purchase(const QString &id, const QString &sku, const QString &name, const QString &metadata);

    //invokable purchases query from the qml control
    Q_INVOKABLE void getExisting();

    //invokable price query from the qml control
    Q_INVOKABLE void getPrice(const QString &id, const QString &sku);

    //invokable subscription terms query from the qml control
    Q_INVOKABLE void getSubscriptionTerms(const QString &id, const QString &sku);

    //invokable subscription status query from the qml control
    Q_INVOKABLE void checkSubscriptionStatus(const QString &id, const QString &sku);

    //invokable subscription cancellation from the qml control
    Q_INVOKABLE void cancelSubscription(const QString &purchaseId);

public Q_SLOTS:
    //This method is called whenever a purchase is invoked
    void purchaseResponse();

    //This method is called whenever a purchases query is invoked
    void existingPurchasesResponse();

    //This method is called whenever a price request is made
    void priceResponse();

    //This method is called whenever subscription terms are queried
    void subscriptionTermsResponse();

    //This method is called whenever subscription status checks are performed
    void subscriptionStatusResponse();

    //This method is called whenever subscription cancellations are made
    void cancelSubscriptionResponse();

Q_SIGNALS:
    //This signal is emitted upon successful purchase
    void purchaseResponseSuccess(const QString &receiptString);

    //This signal is emitted upon purchases query success
    void existingPurchasesResponseSuccess(const QString &receiptsString);

    //This signal is emitted upon successful price query
    void priceResponseSuccess(const QString &price);

    //This signal is emitted upon making successful subscription terms query
    void subscriptionTermsResponseSuccess(const QString &price, const QString &initialPeriod, const QString &renewalPrice, const QString &renewalPeriod);

    //This signal is emitted upon successful subscription status checks.
    void checkStatusResponseSuccess(bool status);

    //This signal is emitted upon successful subscription cancellations.
    void cancelSubscriptionResponseSuccess(bool cancelled);

    //This signal is emitted whenever any of the payment service requests generated an error
    void infoResponseError(int errorCode, const QString &errorText);

private:
    bb::platform::PaymentManager *m_paymentManager;
};

#endif
