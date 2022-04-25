#include "transactionpool.h"
#include "Utility/utility.h"

#include <QJsonArray>
#include <QFile>
#include <algorithm>

TransactionPool::TransactionPool()
{

}

TransactionPool::~TransactionPool()
{

}

Transaction* TransactionPool::updateOrAddTransaction(const Transaction& transaction)
{
    auto containTransaction = findById(transaction.m_id);

    if(containTransaction)
        *containTransaction = transaction;
    else
    {
        m_transactions.push_back(transaction);
        containTransaction = &m_transactions.back();
    }

    return containTransaction;
}

Transaction *TransactionPool::find(const QByteArray &pubKey)
{
    auto predicate = [&](const Transaction &item)
    {
        return item.m_input.m_addr == pubKey;
    };
    return findTemplate(predicate);
}

Transaction* TransactionPool::findById(const QByteArray &id)
{
    auto predicate = [&](const Transaction &item)
    {
        return item.m_id == id;
    };
    return findTemplate(predicate);
}

QJsonDocument TransactionPool::toJson() const
{
    QJsonArray array;
    for(const auto &item : m_transactions)
        array.append(item.toJson().toVariant().toJsonValue());

    QFile f("temp.json");
    f.open(QIODevice::WriteOnly);
    f.write(QJsonDocument(array).toJson());
    return QJsonDocument(array);
}

bool TransactionPool::verifyTransactions() const
{
    for(const auto &item : m_transactions)
        if(!utility_blockchain::verifyTransaction(item))
            return false;
    return true;
}
