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

    static Block createGeneric()
    {
        Block block;
        block.m_data = "-----";
        block.m_time = QDateTime::currentMSecsSinceEpoch();

        calcAndSetHash(block);

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

    std::vector<Transaction> getTransactions() const
    {
        std::vector<Transaction> transactions;
        auto array = QJsonDocument::fromJson(m_data).array();
        for(auto item : array)
            transactions.push_back(Transaction::createTransaction(item.toObject()));
        return transactions;
    }

    QByteArray m_data;
    QByteArray m_hash;
    QByteArray m_prevHash;
    uint64_t m_time = 0;
    uint64_t m_nonce = 0;
    uint16_t m_difficulty = 2;

private:
    static inline uint32_t MIME_RATE = 10000; //3 sec

    static uint16_t adjustDifficulty(const Block &prev_block, uint64_t time)
    {
        return prev_block.m_difficulty + (prev_block.m_time + MIME_RATE > time ? 1 : - 1);
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
