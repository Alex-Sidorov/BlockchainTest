#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QJsonDocument>

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

    QJsonDocument inputJSON() const;
    QJsonDocument outputJSON() const;

    InputTransactionBody m_input;
    std::vector<TransactionBody> m_output;
    uint64_t m_id = 0;
};

#endif // TRANSACTION_H
