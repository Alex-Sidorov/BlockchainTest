#ifndef TRANSACTIONPOOL_H
#define TRANSACTIONPOOL_H

#include "Transaction/transaction.h"

#include <vector>

class TransactionPool
{
public:
    TransactionPool();
    virtual ~TransactionPool();

    Transaction* updateOrAddTransaction(const Transaction &transaction);

    Transaction* find(const QByteArray &pubKey);
    Transaction* findById(const QByteArray &id);

    QJsonDocument toJson() const;

    bool verifyTransactions() const;

private:
    std::vector<Transaction> m_transactions;

    template<typename L>
    Transaction* findTemplate(L &lambda)
    {
        auto containTransaction = std::find_if(m_transactions.begin(), m_transactions.end(), lambda);
        return containTransaction != m_transactions.end() ? &(*containTransaction) : nullptr;
    }

};

#endif // TRANSACTIONPOOL_H
