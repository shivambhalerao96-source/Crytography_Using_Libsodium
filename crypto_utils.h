#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <sodium.h>
#include <iostream>
#include <vector>
#include <string>

// 32-byte shared secret key (Must be identical on Client and Server)
const unsigned char SHARED_KEY[crypto_secretbox_KEYBYTES] = "my_super_secret_32_byte_key_123";

// Encrypts plaintext and packs it into: [Nonce][MAC + Ciphertext]
std::vector<unsigned char> encrypt_payload(const std::string& plaintext) {
    // 1. Generate a random 24-byte nonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    // 2. Allocate space for the ciphertext (which includes the 16-byte MAC)
    std::vector<unsigned char> ciphertext(plaintext.length() + crypto_secretbox_MACBYTES);

    // 3. Encrypt and authenticate in one shot
    crypto_secretbox_easy(ciphertext.data(), 
                          reinterpret_cast<const unsigned char*>(plaintext.data()), 
                          plaintext.length(), 
                          nonce, 
                          SHARED_KEY);

    // 4. Pack the Nonce and the Ciphertext together for transmission
    std::vector<unsigned char> final_payload;
    final_payload.insert(final_payload.end(), nonce, nonce + sizeof nonce);
    final_payload.insert(final_payload.end(), ciphertext.begin(), ciphertext.end());

    return final_payload;
}

// Unpacks [Nonce][Ciphertext] and decrypts. Returns empty string if tampered.
std::string decrypt_payload(const std::vector<unsigned char>& payload, int payload_len) {
    // Check if the packet is at least big enough to hold a nonce and a MAC
    if (payload_len < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
        return ""; 
    }

    // 1. Extract the nonce from the front of the packet
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    std::copy(payload.begin(), payload.begin() + crypto_secretbox_NONCEBYTES, nonce);
    
    // 2. Calculate ciphertext size and setup decryption buffer
    int ciphertext_len = payload_len - crypto_secretbox_NONCEBYTES;
    std::vector<unsigned char> decrypted(ciphertext_len - crypto_secretbox_MACBYTES);

    // 3. Decrypt and verify. Fails if the data was tampered with!
    if (crypto_secretbox_open_easy(decrypted.data(), 
                                   payload.data() + crypto_secretbox_NONCEBYTES, 
                                   ciphertext_len, 
                                   nonce, 
                                   SHARED_KEY) != 0) {
        return ""; // Message forged or corrupted
    }

    return std::string(decrypted.begin(), decrypted.end());
}

#endif 