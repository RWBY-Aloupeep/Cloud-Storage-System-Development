#include "aescbc.h"

AESCBC::AESCBC(QObject *parent) : QObject(parent)
{

}

// 已知 AES_BLOCK_SIZE = 16； SHA256_DIGEST_LENGTH = 32

/**
 * 生成 AES 密钥
 * @param input_key 明文密钥
 * @param aes_key 生成的密钥
 * @param key_len 最终密钥长度
 */
void AESCBC:: generateAESKey(const std::string& input_key, unsigned char* aes_key, int key_len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input_key.c_str()), input_key.size(), hash);
    memcpy(aes_key, hash, key_len);
}


/**
 * AES CBC 加密函数
 * @param plain_text 待加密数据
 */
std::vector<unsigned char> AESCBC::aesEncrypt(const std::string& plain_text){

    unsigned int plain_text_len = plain_text.size();
    unsigned char key_hex[16];
    generateAESKey(KEY_STR, key_hex, 16); //16字节的密码长度
    AES_KEY en_key;
    // ============= 设置 AES 加密密钥 ==================
    if(AES_set_encrypt_key(key_hex,128,&en_key) < 0){
        // 密钥设置失败
        qDebug() << "[AESUtilities] set encrypt key failed!";
    }

    // 计算填充长度
    int padding_len = AES_BLOCK_SIZE - (plain_text_len % AES_BLOCK_SIZE);
    int total_len = plain_text_len + padding_len;

    unsigned char* padded_plain_text = (unsigned char*)malloc(total_len);
    // 使用 memcpy 复制 plain_text 到 padded_plain_text
    memcpy(padded_plain_text, plain_text.c_str(), plain_text_len);
    memset(padded_plain_text + plain_text_len, padding_len, padding_len);

    unsigned char iv_tmp[AES_BLOCK_SIZE]; // AES CBC 模式的 IV 必须是 16 字节, 即 AES_BLOCK_SIZE。
    if (!RAND_bytes(iv_tmp, sizeof(iv_tmp))) {
        qDebug() << "Error generating random IV!";
    }

    // 创建存储加密文本的 vector，额外存储 IV（16 字节）
    std::vector<unsigned char> encrypt_text(total_len + AES_BLOCK_SIZE);

    memcpy(encrypt_text.data(), iv_tmp, AES_BLOCK_SIZE);

    // 执行 AES CBC 加密操作，将密文存储在 encrypt_text 的 16 字节之后
    AES_cbc_encrypt(padded_plain_text, encrypt_text.data() + AES_BLOCK_SIZE, total_len, &en_key, iv_tmp, AES_ENCRYPT);

    // 释放申请的内存
    free(padded_plain_text);
    return encrypt_text;
}


std::vector<unsigned char> AESCBC::aesDecrypt(std::vector<unsigned char> encrypt_text) {
    unsigned char key_hex[16];
    generateAESKey(KEY_STR, key_hex, 16);

    AES_KEY de_key;
    // 设置 AES 解密密钥
    if (AES_set_decrypt_key(key_hex, 128, &de_key) < 0) {
        qDebug() << "[AESUtilities] set decrypt key failed!";
    }

    unsigned char iv_tmp[AES_BLOCK_SIZE];
    // 提取加密文本的前 16 字节作为 IV
    memcpy(iv_tmp, encrypt_text.data(), AES_BLOCK_SIZE);

    std::vector<unsigned char> encrypted_data(encrypt_text.begin() + AES_BLOCK_SIZE, encrypt_text.end());

    // 创建存储解密文本的 vector
    std::vector<unsigned char> decrypt_text(encrypted_data.size());

    // 执行 AES CBC 解密操作
    AES_cbc_encrypt(encrypted_data.data(), decrypt_text.data(), encrypted_data.size(), &de_key, iv_tmp, AES_DECRYPT);

    // 获取填充长度并去除填充
    int padding_len = decrypt_text[decrypt_text.size() - 1];
    decrypt_text.resize(decrypt_text.size() - padding_len);

    return decrypt_text;

}
