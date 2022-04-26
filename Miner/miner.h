#ifndef MINER_H
#define MINER_H

#include "Block/blockchain.h"
#include "Transaction/transactionpool.h"
#include "Wallet/wallet.h"

class Miner
{
public:
    Miner(BlockChain *blockchain, TransactionPool* pool, Wallet *wallet);
    virtual ~Miner();

    Block* mine();

private:

    BlockChain* m_blockchain;
    TransactionPool* m_pool;
    Wallet* m_wallet;
};

#endif // MINER_H
