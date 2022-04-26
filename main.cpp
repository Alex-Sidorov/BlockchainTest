#include <QCoreApplication>
#include <iostream>

#include "Block\blockchain.h"
#include "Transaction/transaction.h"
#include "Transaction/transactionpool.h"
#include "Utility/utility.h"
#include "Miner/miner.h"

#include <QFile>
#include <QJsonDocument>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BlockChain blockchain;

    Wallet wallet;
    wallet.m_balance = 150;
    wallet.m_pairKeys = utility_blockchain::getKeys();
    wallet.m_publicKey = wallet.m_pairKeys.first;

    Wallet wallet2;
    wallet2.m_balance = 100;
    wallet2.m_pairKeys = utility_blockchain::getKeys();
    wallet2.m_publicKey = wallet2.m_pairKeys.first;

    TransactionPool pool;

    wallet.createTransaction(pool, "qwerttre", 100);
    wallet.createTransaction(pool, "qwerttre", 10);


    wallet2.createTransaction(pool, "qwerttre", 10);

    Miner miner(&blockchain, &pool, &wallet);
    auto block = miner.mine();

    QFile file("temp2.json");
    file.open(QIODevice::WriteOnly);
    file.write(blockchain.toJson().toJson());

    std::cout << "Check pool: " << (pool.verifyTransactions() ? "True" : "False") << std::endl;
    std::cout << "Count valid transactions: " << pool.validTransactions().size();

    return a.exec();
}
