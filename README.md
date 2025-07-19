# RSA Encryption / Decryption in C++

This project implements basic RSA encryption and decryption using Boost's Multiprecision library.

### What it does

- Reads plaintext from `*_in.dat`
- Encrypts it using RSA (with `*_keymat.txt`)
- Decrypts it and verifies correctness
- Logs details to `*_debug.txt`

### Requirements

- C++17 or higher
- [Boost 1.70+](https://www.boost.org/) (header-only)
- A C++ compiler (e.g., `g++`, `clang++`)

### How to Compile

1. Download Boost and extract it: https://www.boost.org/users/download/
Your folder structure should include a `boost/` folder with `multiprecision/cpp_int.hpp` inside.
2. Compile the code:
```bash
g++ -std=c++17 -I"path/to/boost" src/RSA.cpp -o rsa_exec
```

### How to Run

1. ./rsa_exec

### It will:
- Use input files like 0_keymat.txt, 0_in.dat, etc.
- Create output files: 0_encrypted.txt, 0_decrypted.dat, and 0_debug.txt
- Repeat for inputs 0 to 3
- Verify that encryption/decryption worked

### Included Test Files

- 4 key files: 0_keymat.txt, ..., 3_keymat.txt
- 4 input files: 0_in.dat, ..., 3_in.dat
