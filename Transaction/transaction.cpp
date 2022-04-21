#include "transaction.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

void Transaction::signTransaction(Transaction &transaction, const Wallet &senderWallet)
{
    transaction.m_input.m_addr = senderWallet.m_publicKey;
    transaction.m_input.m_amount = senderWallet.m_ballance;
    transaction.m_input.m_timestamp = QDateTime::currentMSecsSinceEpoch();
    //transaction.m_input.m_signature = sign(transaction.outputJSON());
}

Transaction Transaction::createTransaction(const Wallet &senderWallet, const QByteArray &recipient, uint64_t amount)
{
    Transaction transaction;
    if(senderWallet.m_ballance < amount)
        return transaction;

    Transaction::TransactionBody firstRecord;
    firstRecord.m_amount = senderWallet.m_ballance - amount;
    firstRecord.m_addr =  senderWallet.m_publicKey;

    Transaction::TransactionBody secondRecord;
    secondRecord.m_amount = amount;
    secondRecord.m_addr =  recipient;

    transaction.m_output.push_back(firstRecord);
    transaction.m_output.push_back(secondRecord);

    signTransaction(transaction, senderWallet);
    transaction.m_id = 1; //TODO
    return transaction;
}

QJsonDocument Transaction::inputJSON() const
{
    QJsonObject json
    {
        qMakePair("amount", m_input.m_amount),
        qMakePair("address", m_input.m_addr),
        qMakePair("signature", m_input.m_signature),
        qMakePair("timestamp", m_input.m_timestamp)
    };
    return QJsonDocument(json);
}

QJsonDocument Transaction::outputJSON() const
{
    QJsonArray array;
    for(const auto& item : m_output)
        array.push_back(QJsonObject({qMakePair("amount", item.m_amount),
                                     qMakePair("address", item.m_addr)}));
    return QJsonDocument(array);
}
