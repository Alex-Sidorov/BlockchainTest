#include <QCoreApplication>
#include <iostream>

#include "Block\blockchain.h"
#include "Transaction/transaction.h"
#include "Transaction/transactionpool.h"
#include "Utility/utility.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*BlockChain blockchain;

    for(int i = 0; i < 3; ++i)
    {
         auto block = blockchain.addBlockIfCorrect(QString::number(i).toLatin1());
         if(!block)
            break;
    }
    std::cout << blockchain.getString().toStdString();*/

    Wallet wallet;
    wallet.m_ballance = 150;
    wallet.m_pairKeys = utility_blockchain::getKeys();
    wallet.m_publicKey = wallet.m_pairKeys.first;

    Wallet wallet2;
    wallet2.m_ballance = 100;
    wallet2.m_pairKeys = utility_blockchain::getKeys();
    wallet2.m_publicKey = wallet2.m_pairKeys.first;

    TransactionPool pool;

    wallet.createTransaction(pool, "qwerttre", 100);
    wallet.createTransaction(pool, "qwerttre", 10);


    wallet2.createTransaction(pool, "qwerttre", 10);

    pool.toJson();

    std::cout << "Check pool: " << (pool.verifyTransactions() ? "True" : "False");

    return a.exec();
}
