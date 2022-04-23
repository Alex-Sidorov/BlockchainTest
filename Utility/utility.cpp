#include "utility.h"
#include "RsaEncryption/qrsaencryption.h"

QPair<QByteArray, QByteArray> utility_blockchain::getKeys()
{
    QRSAEncryption rsa;
    QPair<QByteArray, QByteArray> keys;
    rsa.generatePairKey(keys.first, keys.second);
    return keys;
}

QByteArray utility_blockchain::sign(const QByteArray &data, const QByteArray &privKey)
{
    return QRSAEncryption::signMessage(data, privKey, QRSAEncryption::RSA_2048);
}

bool utility_blockchain::verifyTransaction(const Transaction& transaction)
{
    return QRSAEncryption::checkSignMessage(transaction.outputJSON().toBinaryData(),
                                            transaction.m_input.m_signature,
                                            transaction.m_input.m_addr,
                                            QRSAEncryption::RSA_2048);
}
