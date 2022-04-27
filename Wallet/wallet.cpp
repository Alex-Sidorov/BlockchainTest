#include "wallet.h"
#include "Transaction/transactionpool.h"
#include "Block/blockchain.h"

Transaction *Wallet::createTransaction(TransactionPool &pool, const BlockChain &blockChain, const QByteArray &recipient, uint64_t amount)
{
    auto newBalance = calcBalance(blockChain);
    if(newBalance.second)
        m_balance = newBalance.first;

    if(amount > m_balance)
        return nullptr;

    Transaction* transaction = pool.find(m_publicKey);
    if(transaction)
        transaction->update(*this, recipient, amount);
    else
        transaction = pool.updateOrAddTransaction(Transaction::createTransaction(*this, recipient, amount));
    return transaction;
}

QPair<uint64_t, bool> Wallet::calcBalance(const BlockChain &blockChain)
{
    auto transactions = blockChain.getTransactions();
    if(transactions.empty())
        return qMakePair(0, false);

    std::vector<const Transaction*> otherTransactions;
    otherTransactions.reserve(transactions.size());

    const Transaction* recentInput = nullptr;
    for(const auto& item : transactions)
    {
        if(item.m_input.m_addr == m_publicKey
                && (!recentInput
                    || recentInput->m_input.m_timestamp < item.m_input.m_timestamp))
            recentInput = &item;
        else
            otherTransactions.push_back(&item);
    }

    uint64_t balance = m_balance;
    uint64_t startTime = 0;
    if(recentInput)
    {
        startTime = recentInput->m_input.m_timestamp;
        balance = std::find_if(recentInput->m_output.begin(), recentInput->m_output.end(),
                                            [&](const Transaction::TransactionBody &item)
        {
            return item.m_addr == m_publicKey;
        })->m_amount;
    }

    for(const auto& item : otherTransactions)
    {
        if(item->m_input.m_timestamp > startTime)
        {
            for(const auto& output : item->m_output)
            {
                if(output.m_addr == m_publicKey)
                    balance += output.m_amount;
            }
        }
    }
    return qMakePair(balance, true);
}
