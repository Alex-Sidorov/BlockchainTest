 #include "transactionpool.h"
#include "Utility/utility.h"

#include <QJsonArray>
#include <algorithm>

TransactionPool::TransactionPool()
{}

TransactionPool::~TransactionPool()
{}

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
    return toJson(m_transactions);
}

QJsonDocument TransactionPool::toJson(const std::vector<Transaction>& transactions)
{
    QJsonArray array;
    for(const auto &item : transactions)
        array.append(item.toJson().toVariant().toJsonValue());

    return QJsonDocument(array);
}

bool TransactionPool::verifyTransaction(const Transaction& transaction) const
{
    uint64_t total = 0;
    for(auto& item : transaction.m_output)
        total += item.m_amount;

    return transaction.m_input.m_amount == total
            && utility_blockchain::verifyTransaction(transaction);
}

bool TransactionPool::verifyTransactions() const
{
    for(const auto &item : m_transactions)
        if(!verifyTransaction(item))
            return false;

    return true;
}

std::vector<Transaction> TransactionPool::validTransactions()
{
    std::vector<Transaction> transactions;
    transactions.reserve(m_transactions.size());

    for(auto &item : m_transactions)
        if(verifyTransaction(item))
            transactions.push_back(item);

    return transactions;
}

void TransactionPool::clear()
{
    m_transactions.clear();
}
