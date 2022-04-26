#ifndef UTILITY_H
#define UTILITY_H

#include <QPair>
#include <QByteArray>

struct Transaction;

namespace utility_blockchain
{
    QPair<QByteArray, QByteArray> getKeys();

    QByteArray sign(const QByteArray& data, const QByteArray &privKey);

    bool verifyTransaction(const Transaction& transaction);

    QByteArray uuid();
}

#endif
