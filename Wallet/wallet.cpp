#include "wallet.h"
#include "Transaction/transactionpool.h"

Transaction *Wallet::createTransaction(TransactionPool &pool, const QByteArray &recipient, uint64_t amount)
{
    if(amount > m_ballance)
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
    return "Ballance: " + QString::number(m_ballance)
            + "Public Key: " + m_publicKey
            + "Private Key: " + m_pairKeys.second;
}
