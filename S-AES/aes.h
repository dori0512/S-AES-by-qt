#ifndef AES_H
#define AES_H

#include <vector>
#include <QString>

class AES {
public:
    AES();

    QString encrypt(const QString& plaintext, const QString& key);
    QString decrypt(const QString& ciphertext, const QString& key);

private:
    std::vector<std::vector<int>> sbox;
    std::vector<std::vector<int>> invSbox;
//    std::vector<int> rcon_1;
    QString rcon_1;
    QString rcon_2;
//    std::vector<int> rcon_2;
    std::vector<std::vector<int>> roundKey;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<int>> invmatrix;

    QString substituteBytes(const QString& state, bool inverse = false);
    QString shiftRows(const QString& state, bool inverse = false);
    QString mixColumns(const QString& state, bool inverse = false);
    QString addRoundKey(const QString& state, const QString& key);
    QString expendkey(const QString& key, int round);

    QString encryptBlock(const QString& plaintext, const QString& key);
    QString decryptBlock(const QString& ciphertext, const QString& key);

    QString decToBin(int num, int width);
    int binToDec(const QString& binStr);
    int multiply(int num1, int num2);
    void x_de_n_fang_cheng_fx(int xfx[4], int a[4]);
    QString roundKeyToString(const std::vector<int>& roundKey);
    QString addBinary(const QString& bin1, const QString& bin2);
};

#endif // AES_H
