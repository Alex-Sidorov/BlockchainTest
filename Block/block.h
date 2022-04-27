#ifndef BLOCK_H
#define BLOCK_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <vector>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>
#include <QJsonDocument>

#include "Transaction/transactionpool.h"
#include "Transaction/transaction.h"

struct Block
{
    bool isValid(const Block& prev_block) const
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        crypto.addData(m_data);
        crypto.addData(prev_block.m_hash);
        crypto.addData(QByteArray::number(m_time));
        crypto.addData(QByteArray::number(m_nonce));
        crypto.addData(QByteArray::number(m_difficulty));
        return m_hash == crypto.result();
    }

    static Block mineBlock(const QByteArray &data, const Block& prev_block)
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        Block block;
        block.m_data = data;
        block.m_prevHash = prev_block.m_hash;

        do
        {
            block.m_nonce += 1;
            block.m_time = QDateTime::currentMSecsSinceEpoch();
            block.m_difficulty = Block::adjustDifficulty(prev_block, block.m_time);

            calcAndSetHash(block);

        }while(block.m_hash.mid(0, block.m_difficulty) != QByteArray().insert(0, block.m_difficulty, '0'));

        return block;
    }

    static Block mineBlock(const std::vector<Transaction>& data, const Block& prev_block)
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        Block block;
        block.m_transactions = data;
        block.m_data = TransactionPool::toJson(data).toJson();
        block.m_prevHash = prev_block.m_hash;

        do
        {
            block.m_nonce += 1;
            block.m_time = QDateTime::currentMSecsSinceEpoch();
            block.m_difficulty = Block::adjustDifficulty(prev_block, block.m_time);

            calcAndSetHash(block);

        }while(block.m_hash.mid(0, block.m_difficulty).count('0') != block.m_difficulty);

        return block;
    }

    static Block createGeneric()
    {
        Block block;
        block.m_data = "-----";

        do
        {
            block.m_time = QDateTime::currentMSecsSinceEpoch();
            block.m_nonce += 1;
            block.m_time = QDateTime::currentMSecsSinceEpoch();
            calcAndSetHash(block);

        }while(block.m_hash.mid(0, block.m_difficulty).count('0') != block.m_difficulty);

        return block;
    }

    QJsonDocument toJson() const
    {
        QJsonObject object;
        object.insert("time", QJsonValue::fromVariant(QVariant::fromValue(m_time)));
        object.insert("difficulty", QJsonValue::fromVariant(QVariant::fromValue(m_difficulty)));
        object.insert("nonce", QJsonValue::fromVariant(QVariant::fromValue(m_nonce)));
        object.insert("hash", QJsonValue::fromVariant(QVariant::fromValue(m_hash)));
        object.insert("prevHash", QJsonValue::fromVariant(QVariant::fromValue(m_prevHash)));
        object.insert("data", QJsonValue(QJsonDocument::fromJson(m_data).array()));
        return QJsonDocument(object);
    }

    const std::vector<Transaction>& getTransactions() const
    {
        return m_transactions;
    }

    QByteArray m_data;
    QByteArray m_hash;
    QByteArray m_prevHash;
    uint64_t m_time = 0;
    uint64_t m_nonce = 0;
    uint16_t m_difficulty = 2;

    std::vector<Transaction> m_transactions;

private:
    static inline uint32_t MIME_RATE = 10000; //3 sec

    static uint16_t adjustDifficulty(const Block &prev_block, uint64_t time)
    {
        if(prev_block.m_time + MIME_RATE > time)
            return prev_block.m_difficulty + 1;
        else if(prev_block.m_difficulty - 1)
            return prev_block.m_difficulty - 1;
        else
            return prev_block.m_difficulty;
    }

    static void calcAndSetHash(Block &block)
    {
        QCryptographicHash crypto(QCryptographicHash::Sha256);
        crypto.addData(block.m_data);
        crypto.addData(block.m_prevHash);
        crypto.addData(QByteArray::number(block.m_time));
        crypto.addData(QByteArray::number(block.m_nonce));
        crypto.addData(QByteArray::number(block.m_difficulty));
        block.m_hash = crypto.result();
    }
};

#endif // BLOCK_H
