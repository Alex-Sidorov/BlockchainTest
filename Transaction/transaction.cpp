#include "transaction.h"
#include "Utility/utility.h"
#include "Wallet/wallet.h"

#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>
#include <QPair>

void Transaction::signTransaction(Transaction &transaction, const Wallet &senderWallet)
{
    transaction.m_input.m_addr = senderWallet.m_publicKey;
    transaction.m_input.m_amount = senderWallet.m_balance;
    transaction.m_input.m_timestamp = QDateTime::currentMSecsSinceEpoch();
    transaction.m_input.m_signature = utility_blockchain::sign(transaction.outputJSON().toBinaryData(), senderWallet.m_pairKeys.second);
}

Transaction Transaction::createTransaction(const Wallet &senderWallet, const QByteArray &recipient, uint64_t amount)
{
    Transaction transaction;
    if(senderWallet.m_balance < amount)
        return transaction;

    Transaction::TransactionBody firstRecord;
    firstRecord.m_amount = senderWallet.m_balance - amount;
    firstRecord.m_addr =  senderWallet.m_publicKey;

    Transaction::TransactionBody secondRecord;
    secondRecord.m_amount = amount;
    secondRecord.m_addr =  recipient;

    transaction.m_output.push_back(firstRecord);
    transaction.m_output.push_back(secondRecord);

    transaction.m_id = utility_blockchain::uuid();

    signTransaction(transaction, senderWallet);
    return transaction;
}

Transaction Transaction::createTransaction(const Wallet &senderWallet, const std::vector<TransactionBody> &output)
{
    Transaction transaction;

    transaction.m_id = utility_blockchain::uuid();
    transaction.m_output = output;

    signTransaction(transaction, senderWallet);

    return transaction;
}

Transaction Transaction::createTransaction(const QJsonObject &obj)
{
    Transaction transaction;

    transaction.m_id = obj["id"].toVariant().toByteArray();

    auto input = obj["input"].toObject();

    transaction.m_input.m_addr = QByteArray::fromHex(input["address"].toVariant().toByteArray());
    transaction.m_input.m_amount = input["amount"].toVariant().toULongLong();
    transaction.m_input.m_signature = input["signature"].toVariant().toByteArray();
    transaction.m_input.m_timestamp = input["timestamp"].toVariant().toULongLong();


    QJsonArray array = obj["output"].toArray();
    transaction.m_output.reserve(array.size());
    for(const auto& item : array)
    {
        auto body = item.toObject();
        transaction.m_output.push_back(TransactionBody());
        transaction.m_output.back().m_addr = QByteArray::fromHex(body["address"].toVariant().toByteArray());
        transaction.m_output.back().m_amount = body["amount"].toVariant().toULongLong();
    }
    return transaction;
};

Transaction Transaction::rewardTransaction(const Wallet &minerWallet,
                                           const Wallet &blockchainWallet,
                                           uint64_t reward)
{
    Transaction::TransactionBody record;
    record.m_amount = reward;
    record.m_addr = minerWallet.m_publicKey;
    return createTransaction(blockchainWallet, {record});
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
        {"address", QJsonValue::fromVariant(QVariant::fromValue(m_input.m_addr.toHex()))},
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
        obj.insert("address",QJsonValue::fromVariant(QVariant::fromValue(item.m_addr.toHex())));
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
