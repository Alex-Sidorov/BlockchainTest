#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QJsonDocument>

struct Wallet;

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

    static Transaction createTransaction(const Wallet& senderWallet,
                                         const std::vector<TransactionBody>& output);

    static Transaction createTransaction(const QJsonObject &obj);

    static Transaction rewardTransaction(const Wallet& minerWallet,
                                         const Wallet& blockchainWallet,
                                         uint64_t reward = 10/*TODO*/);

    bool update(const Wallet& senderWallet,
                const QByteArray& recipient,
                uint64_t amount);

    Transaction &operator=(const Transaction &transaction);

    QJsonDocument inputJSON() const;
    QJsonDocument outputJSON() const;

    QJsonDocument toJson() const;

    InputTransactionBody m_input;
    std::vector<TransactionBody> m_output;
    QByteArray m_id;

};

#endif // TRANSACTION_H
