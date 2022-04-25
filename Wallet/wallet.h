#ifndef WALLET_H
#define WALLET_H

#include <QByteArray>
#include <QPair>
#include <QString>

class TransactionPool;
struct Transaction;

struct Wallet
{
    QPair<QByteArray, QByteArray> m_pairKeys;
    QByteArray m_publicKey;
    uint64_t m_ballance;

    QString toString() const;


    Transaction* createTransaction(TransactionPool& pool,
                                   const QByteArray& recipient,
                                   uint64_t amount);
};

#endif // WALLET_H
