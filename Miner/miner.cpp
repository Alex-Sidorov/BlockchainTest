#include "miner.h"

Miner::Miner(BlockChain *blockchain, TransactionPool* pool, Wallet *wallet) :
    m_blockchain(blockchain),
    m_pool(pool),
    m_wallet(wallet)
{

}

Miner::~Miner()
{}


Block* Miner::mine()
{
    if(!m_pool || !m_wallet || !m_blockchain)
        return nullptr;

    auto validTransactions = m_pool->validTransactions();
    validTransactions.push_back(Transaction::rewardTransaction(*m_wallet, m_blockchainWallet));

    auto block = m_blockchain->addBlock(validTransactions);

    m_pool->clear();
    //TODO sync with any nodes
    return block;
}
