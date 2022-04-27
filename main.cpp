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


    Miner miner(&blockchain, &pool, &wallet);
    Miner miner2(&blockchain, &pool, &wallet2);

    wallet.createTransaction(pool, blockchain, "qwerttre", 100);
    miner.mine();
    wallet.createTransaction(pool, blockchain, "qwerttre", 10);
    miner2.mine();


    wallet2.createTransaction(pool, blockchain, "qwerttre", 10);
    miner.mine();

    QFile file("temp2.json");
    file.open(QIODevice::WriteOnly);
    file.write(blockchain.toJson().toJson());
    file.close();

    std::cout << "Check BlokChain: " << (blockchain.checkBlockChain() ?  "True" : "False");

    return a.exec();
}
