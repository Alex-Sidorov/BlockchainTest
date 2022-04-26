#ifndef WALLET_H
#define WALLET_H

#include <QByteArray>
#include <QPair>
#include <QString>

#include "Utility/utility.h"
#include "Block/blockchain.h"

class BlockChain;
class TransactionPool;
struct Transaction;

struct Wallet
{
    QPair<QByteArray, QByteArray> m_pairKeys;
    QByteArray m_publicKey;
    uint64_t m_balance;

    QString toString() const;

    void calcBalance(const BlockChain& blockChain);

    //TEST
    static Wallet blockChainWallet()
    {
        Wallet wallet;
        wallet.m_balance = 100;
        wallet.m_pairKeys = utility_blockchain::getKeys();
        wallet.m_publicKey = wallet.m_pairKeys.first;
        return wallet;
    }


    Transaction* createTransaction(TransactionPool& pool,
                                   const QByteArray& recipient,
                                   uint64_t amount);
};

#endif // WALLET_H
