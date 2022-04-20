#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"

class BlockChain
{

public:
    BlockChain(bool isEmpty = true);
    BlockChain(const BlockChain &) = delete;
    BlockChain(const BlockChain &&) = delete;
    virtual ~BlockChain();

    Block* addBlock(const QByteArray &data);

    bool checkBlockChain() const;
    Block* addBlockIfCorrect(const QByteArray &data);

    QString getString() const;

private:
    std::vector<Block> m_blockchain;
};

#endif // BLOCKCHAIN_H
