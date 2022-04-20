#include <QCoreApplication>
#include <iostream>

#include "Block\blockchain.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BlockChain blockchain;

    for(int i = 0; i < 3; ++i)
    {
         auto block = blockchain.addBlockIfCorrect(QString::number(i).toLatin1());
         if(!block)
            break;
    }
    std::cout << blockchain.getString().toStdString();

    return a.exec();
}
