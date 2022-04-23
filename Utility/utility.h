#ifndef UTILITY_H
#define UTILITY_H

#include "../Transaction/transaction.h"

#include <QPair>
#include <QByteArray>

namespace utility_blockchain
{
    QPair<QByteArray, QByteArray> getKeys();

    QByteArray sign(const QByteArray& data, const QByteArray &privKey);

    bool verifyTransaction(const Transaction& transaction);
}

#endif
