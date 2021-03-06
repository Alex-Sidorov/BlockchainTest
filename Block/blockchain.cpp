#include "blockchain.h"
#include "Transaction/transaction.h"

#include <QJsonArray>

BlockChain::BlockChain(bool isEmpty)
{
    if(isEmpty)
    {
        m_blockchain.emplace_back(Block::createGeneric());
    }
    else
    {
        //TODO READ BLOCKCHAIN
    }
}

BlockChain::~BlockChain()
{}

Block* BlockChain::addBlock(const QByteArray &data)
{
    if(m_blockchain.empty())
        return nullptr;

    m_blockchain.push_back(Block::mineBlock(data, m_blockchain.back()));
    return &m_blockchain.back();
}

Block *BlockChain::addBlock(const std::vector<Transaction> &data)
{
    if(m_blockchain.empty())
        return nullptr;

    m_blockchain.push_back(Block::mineBlock(data, m_blockchain.back()));
    return &m_blockchain.back();
}

bool BlockChain::checkBlockChain() const
{
    for(size_t i = 1; i < m_blockchain.size(); ++i)
    {
        if(!m_blockchain[i].isValid(m_blockchain[i - 1]))
            return false;
    }
    return true;
}

Block* BlockChain::addBlockIfCorrect(const QByteArray &data)
{
    if(!checkBlockChain())
        return nullptr;
    else
        return addBlock(data);
}

std::vector<Transaction> BlockChain::getTransactions() const
{
    std::vector<Transaction> transactions;
    for(const auto &item : m_blockchain)
    {
        auto temp = item.getTransactions();
        std::move(temp.begin(), temp.end(), std::back_inserter(transactions));
    }
    return transactions;
}

QJsonDocument BlockChain::toJson() const
{
    QJsonArray array;
    for(const auto &item : m_blockchain)
        array.append(item.toJson().toVariant().toJsonValue());

    return QJsonDocument(array);
}

QString BlockChain::getString() const
{
    QString result;

    for(size_t i = 0; i < m_blockchain.size(); ++i)
    {
        result += "Data: " + m_blockchain[i].m_data + "\n";
        result += "Hash: " + m_blockchain[i].m_hash + "\n";
        result += "Previus hash: " + m_blockchain[i].m_prevHash + "\n";
        result += "Nonce: " + QString::number(m_blockchain[i].m_nonce) + "\n";
        result += "Difficulty: " + QString::number(m_blockchain[i].m_difficulty) + "\n";

        QDateTime time;
        time.setMSecsSinceEpoch(m_blockchain[i].m_time);
        result += "Time: " + time.toString("yyyy.MM.dd hh:mm:ss") + "\n\n";
    }

    result += QString("Valid: ") + (checkBlockChain() ? "true" : "false");

    return result;
}
