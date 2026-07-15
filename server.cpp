#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include "crypto_utils.h" 

int main() {
    // 1. Initialize Cryptography
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium." << std::endl;
        return 1;
    }

    // 2. Setup UDP Socket Listener
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    std::cout << "Server listening securely on UDP 8080..." << std::endl;

    std::vector<unsigned char> buffer(2048);
    socklen_t client_len = sizeof(client_addr);

    // 3. The Server Loop
    while (true) {
        int nread = recvfrom(server_fd, buffer.data(), buffer.size(), 0, 
                             (struct sockaddr *)&client_addr, &client_len);
        
        if (nread > 0) {
            // Pass the raw byte vector to our utility function
            std::string decrypted_msg = decrypt_payload(buffer, nread);

            if (decrypted_msg.empty()) {
                std::cerr << "[WARNING] Failed to decrypt: Packet corrupted or forged!" << std::endl;
            } else {
                std::cout << "Successfully Decrypted: " << decrypted_msg << std::endl;
            }
        }
    }

    close(server_fd);
    return 0;
}