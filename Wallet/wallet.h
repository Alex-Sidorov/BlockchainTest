#ifndef WALLET_H
#define WALLET_H

#include <QByteArray>
#include <QPair>

struct Wallet
{
    QPair<QByteArray, QByteArray> m_pairKeys;
    QByteArray m_publicKey;
    uint64_t m_ballance;
};

#endif // WALLET_H
