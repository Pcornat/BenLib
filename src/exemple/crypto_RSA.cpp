//////////////////////////////////////////////////////////////
//   ____                                                   //
//  | __ )  ___ _ __  ___ _   _ _ __   ___ _ __ _ __   ___  //
//  |  _ \ / _ \ '_ \/ __| | | | '_ \ / _ \ '__| '_ \ / __| //
//  | |_) |  __/ | | \__ \ |_| | |_) |  __/ |  | |_) | (__  //
//  |____/ \___|_| |_|___/\__,_| .__/ \___|_|  | .__/ \___| //
//                             |_|             |_|          //
//////////////////////////////////////////////////////////////
//                                                          //
//  BenLib, 2020                                            //
//  Created: 22, February, 2021                             //
//  Modified: 22, February, 2021                            //
//  file: crypto.cpp                                        //
//  Crypto                                                  //
//  Source: https://github.com/bavlayan/Encrypt-Decrypt-with-OpenSSL---RSA                                               //
//  OS: ALL                                                 //
//  CPU: ALL                                                //
//                                                          //
//////////////////////////////////////////////////////////////

#include <iostream>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
#include "crypto/crypto_RSA.hpp"

using namespace std;

int main()
{

    LOG("OpenSSL_RSA has been started.");

    RSA *private_key;
    RSA *public_key;
    RSA *keypair = NULL;
    BIGNUM *bne = NULL;
    int ret = 0;
    char message[KEY_LENGTH / 8] = "Batuhan AVLAYAN - OpenSSL_RSA demo";
    char *encrypt = NULL;
    char *decrypt = NULL;

    char private_key_pem[12] = "private_key";
    char public_key_pem[11] = "public_key";

    LOG(KEY_LENGTH);
    LOG(PUBLIC_EXPONENT);

    // RSA *keypair = RSA_generate_key(KEY_LENGTH, PUBLIC_EXPONENT, NULL, NULL); //Old

    bne = BN_new();
    LOG(bne);
    ret = BN_set_word(bne, PUBLIC_EXPONENT);
    if (ret != 1) {
        // goto free_stuff;
        LOG("An error occurred in BN_set_word() method");
    }
    keypair = RSA_new();
    ret = RSA_generate_key_ex(keypair, KEY_LENGTH, bne, NULL);
    if (ret != 1) {
        // goto free_stuff;
        LOG("An error occurred in RSA_generate_key_ex() method");
    }

    LOG("Generate key has been created.");

    private_key = my::crypto::create_RSA(keypair, PRIVATE_KEY_PEM, private_key_pem);
    LOG("Private key pem file has been created.");

    public_key = my::crypto::create_RSA(keypair, PUBLIC_KEY_PEM, public_key_pem);
    LOG("Public key pem file has been created.");
    ;

    encrypt = (char *)malloc((size_t)RSA_size(public_key));
    int encrypt_length
        = my::crypto::public_encrypt(strlen(message) + 1, (unsigned char *)message, (unsigned char *)encrypt, public_key, RSA_PKCS1_OAEP_PADDING);
    if (encrypt_length == -1) {
        LOG("An error occurred in public_encrypt() method");
    }
    LOG("Data has been encrypted.");

    char filename[] = "encrypted_file.bin";
    my::crypto::create_encrypted_file(encrypt, public_key, filename);
    LOG("Encrypted file has been created.");

    decrypt = (char *)malloc((size_t)encrypt_length);
    int decrypt_length = my::crypto::private_decrypt(encrypt_length, (unsigned char *)encrypt, (unsigned char *)decrypt, private_key, RSA_PKCS1_OAEP_PADDING);
    if (decrypt_length == -1) {
        LOG("An error occurred in private_decrypt() method");
    }
    LOG("Data has been decrypted.");

    FILE *decrypted_file = fopen("decrypted_file.txt", "w");
    fwrite(decrypt, sizeof(*decrypt), (size_t)decrypt_length - 1, decrypted_file);
    fclose(decrypted_file);
    LOG("Decrypted file has been created.");

    // free_stuff:
    RSA_free(keypair);
    free(private_key);
    free(public_key);
    free(encrypt);
    free(decrypt);
    BN_free(bne);
    LOG("OpenSSL_RSA has been finished.");

    return 0;
}
