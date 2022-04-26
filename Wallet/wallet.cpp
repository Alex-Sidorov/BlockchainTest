#include "wallet.h"
#include "Transaction/transactionpool.h"
#include "Block/blockchain.h"

Transaction *Wallet::createTransaction(TransactionPool &pool, const QByteArray &recipient, uint64_t amount)
{
    if(amount > m_balance)
        return nullptr;

    Transaction* transaction = pool.find(m_publicKey);
    if(transaction)
        transaction->update(*this, recipient, amount);
    else
        transaction = pool.updateOrAddTransaction(Transaction::createTransaction(*this, recipient, amount));
    return transaction;
}


QString Wallet::toString() const
{
    return "Ballance: " + QString::number(m_balance)
            + "Public Key: " + m_publicKey
            + "Private Key: " + m_pairKeys.second;
}

/*void Wallet::calcBalance(const BlockChain &blockChain)
{
    auto transactions = blockChain.getTransactions();

    uint64_t balance = 0;
    for(auto& item : transactions)
    {




    }
}*/
