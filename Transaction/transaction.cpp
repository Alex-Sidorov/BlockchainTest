#include "transaction.h"
#include "Utility/utility.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>
#include <QPair>

void Transaction::signTransaction(Transaction &transaction, const Wallet &senderWallet)
{
    transaction.m_input.m_addr = senderWallet.m_publicKey;
    transaction.m_input.m_amount = senderWallet.m_ballance;
    transaction.m_input.m_timestamp = QDateTime::currentMSecsSinceEpoch();
    transaction.m_input.m_signature = utility_blockchain::sign(transaction.outputJSON().toBinaryData(), senderWallet.m_pairKeys.second);
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
    transaction.m_id = utility_blockchain::uuid();
    return transaction;
}

bool Transaction::update(const Wallet &senderWallet, const QByteArray &recipient, uint64_t amount)
{
    auto record = std::find_if(m_output.begin(), m_output.end(),[&senderWallet](const TransactionBody &record)
    {
         return senderWallet.m_publicKey == record.m_addr;
    });

    if(record == m_output.end())
        return false;

    if(amount > record->m_amount)
        return false;

    record->m_amount -= amount;

    m_output.push_back(TransactionBody());
    m_output.back().m_amount = amount;
    m_output.back().m_addr = recipient;

    signTransaction(*this, senderWallet);
    return true;
}

Transaction &Transaction::operator=(const Transaction &transaction)
{
    m_input = transaction.m_input;
    m_output = transaction.m_output;
    m_id = transaction.m_id;
    return *this;
}

QJsonDocument Transaction::inputJSON() const
{
    QJsonObject json
    {
        {"amount", QJsonValue::fromVariant(QVariant::fromValue(m_input.m_amount))},
        {"address", QJsonValue::fromVariant(QVariant::fromValue(m_input.m_addr))},
        {"signature", QJsonValue::fromVariant(QVariant::fromValue(m_input.m_signature))},
        {"timestamp", QJsonValue::fromVariant(QVariant::fromValue(m_input.m_timestamp))}
    };
    return QJsonDocument(json);
}

QJsonDocument Transaction::outputJSON() const
{
    QJsonArray array;
    for(const auto& item : m_output)
    {
        QJsonObject obj;
        obj.insert("amount",QJsonValue::fromVariant(QVariant::fromValue(item.m_amount)));
        obj.insert("address",QJsonValue::fromVariant(QVariant::fromValue(item.m_addr)));
        array.push_back(obj);
    }
    return QJsonDocument(array);
}

QJsonDocument Transaction::toJson() const
{
    QJsonObject json;
    json.insert("id", QJsonValue::fromVariant(QVariant::fromValue(m_id)));
    json.insert("input", inputJSON().object());
    json.insert("output", QJsonValue(outputJSON().array()));
    return QJsonDocument(json);
}
