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

    // 2. Setup UDP Socket pointing to localhost (127.0.0.1)
    int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. Create the data payload
    std::string message = "Me hun Gian! Me hun bada takatar! Mera Gala he bohot Surila!";
    std::cout << "Original Message: " << message << std::endl;

    // 4. Encrypt the data using our utility function
    std::vector<unsigned char> secure_packet = encrypt_payload(message);

    // 5. Transmit the encrypted byte array over the network
    sendto(client_fd, secure_packet.data(), secure_packet.size(), 0, 
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    std::cout << "Transmitted " << secure_packet.size() << " encrypted bytes to the server." << std::endl;

    close(client_fd);
    return 0;
}