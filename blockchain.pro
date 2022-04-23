QT -= gui

CONFIG += c++11 c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Block\blockchain.cpp \
        Transaction/transaction.cpp \
        Utility/BigInt/bigint.cpp \
        Utility/BigInt/mini-gmp.c \
        Utility/RsaEncryption/qrsaencryption.cpp \
        Utility/utility.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Block\block.h \
    Block\blockchain.h \
    Transaction/transaction.h \
    Utility/BigInt/bigint.h \
    Utility/BigInt/mini-gmp.h \
    Utility/BigInt/minigmp_global.h \
    Utility/RsaEncryption/qrsaencryption.h \
    Utility/RsaEncryption/qtsecret_global.h \
    Utility/utility.h \
    Wallet/wallet.h
