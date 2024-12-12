#ifndef AES_H
#define AES_H

#include <QObject>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <QDebug>
#include <iostream>

#define KEY_STR "csc3002Group19"

class AESCBC : public QObject
{
    Q_OBJECT
public:
    explicit AESCBC(QObject *parent = nullptr);
    static void generateAESKey(const std::string& input_key, unsigned char* aes_key, int key_len);
    static std::vector<unsigned char> aesEncrypt(const std::string& plain_text);
    static std::vector<unsigned char> aesDecrypt(std::vector<unsigned char> encrypt_text);
private:

signals:

};

#endif // AES_H


