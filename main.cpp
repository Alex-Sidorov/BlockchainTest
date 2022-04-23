#include <QCoreApplication>
#include <iostream>

#include "Block\blockchain.h"
#include "Transaction/Transaction.h"
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

    auto tr = Transaction::createTransaction(wallet, "qwerttre", 100);
    tr.update(wallet, "qwerttre", 10);

    std::cout << "Check Signature: " << (utility_blockchain::verifyTransaction(tr) ? "true" : "false") << std::endl;

    return a.exec();
}
