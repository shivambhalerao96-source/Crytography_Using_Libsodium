# Encrypted UDP Ping 🛡️

A high-performance, cryptographically secure UDP client/server architecture written in C++. This project demonstrates raw socket programming and authenticated encryption by acting as a foundational tunneling engine.

## The Architecture
Unlike standard TCP/IP networking, this engine manually handles encapsulation and encryption:
1. **Cryptography:** Utilizes `libsodium` (XSalsa20-Poly1305) to encrypt payloads and append a 16-byte Message Authentication Code (MAC) to prevent tampering.
2. **Encapsulation:** Wraps the randomized ciphertext and a 24-byte Nonce into a raw UDP packet.
3. **Transmission:** Routes the data over a custom port, completely obfuscating the original payload from network sniffers (like Wireshark).

## Prerequisites
To compile this engine, you must have the `libsodium` development library installed on your system.
* **Linux (Debian/Ubuntu):** `sudo apt-get install libsodium-dev`
* **macOS:** `brew install libsodium`

## Compilation & Execution
Run the commands in seperate terminal

```bash
g++ server.cpp -o server -lsodium
g++ client.cpp -o client -lsodium
```
Run the server first to open the listener, then fire the client (in their respective terminal only)

```bash
./server
./client
```
