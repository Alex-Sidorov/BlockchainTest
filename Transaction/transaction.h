#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QJsonDocument>

#include "Wallet/wallet.h"
struct Transaction
{

    struct TransactionBody
    {
        uint64_t m_amount = 0;
        QByteArray m_addr;
    };

    struct InputTransactionBody : TransactionBody
    {
        uint64_t m_timestamp = 0;
        QByteArray m_signature;
    };

    static void signTransaction(Transaction &transaction, const Wallet& senderWallet);

    static Transaction createTransaction(const Wallet& senderWallet,
                                         const QByteArray& recipient,
                                         uint64_t amount);

    QJsonDocument inputJSON() const;
    QJsonDocument outputJSON() const;

    InputTransactionBody m_input;
    std::vector<TransactionBody> m_output;
    uint64_t m_id = 0;

};

#endif // TRANSACTION_H
