#include <string>
#include <fstream>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <iostream>
#include "password_cracker.h"

void defNotHackerMenu(){
    while(1){
        std::string input;
        std::string hash;
        std::cout << "1. Compare MD5 hash" << std::endl 
                    << "2. Compare SHA256 hash" << std::endl 
                    << "9. Exit" << std::endl;
        std::getline(std::cin, input);

        if(input == "9"){
            return;
        }else if(input == "1"){
            std::cout << "Enter hash: ";
            std::getline(std::cin, hash);
            matchPassword(hash, USE_MD5);

        }else if(input == "2"){
            std::cout << "Enter hash: ";
            std::getline(std::cin, hash);
            matchPassword(hash, USE_SHA256);

        }else{
            std::cout << "Invalid choice" << std::endl;
        }
    }
}

void matchPassword(const std::string hash, const int algorithm){
    std::string foundPassword;
    std::string foundHash;
    std::ifstream inFile;
    if(algorithm == USE_MD5){
        inFile.open("hashesMD5.txt", std::ios::in);
    }else if(algorithm == USE_SHA256){
        inFile.open("hashesSHA.txt", std::ios::in);
    }

    while(inFile){    
        std::getline(inFile, foundPassword, ':');
        std::getline(inFile, foundHash);
        if(foundHash == hash){
            std::cout << "Match found!" << std::endl << hash << " = " << foundPassword << std::endl;
            return;
        }
    }
    std::cout << "Sorry, no match :(" << std::endl;
    inFile.close();
}






void MD5HashFile(){
    std::string line;
    std::ifstream inFile("spicypw.dict", std::ios::in);
    std::ofstream outFile("hashesMD5.txt", std::ios::app);

    while(inFile){    
        std::getline(inFile, line);
        outFile << line << ":" << MD5Hash(line) << std::endl;
    }

    outFile.close();
}

std::string MD5Hash(const std::string& input) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);
    EVP_DigestUpdate(mdctx, input.c_str(), input.length());

    unsigned char digest[MD5_DIGEST_LENGTH];
    unsigned int md_len;

    EVP_DigestFinal_ex(mdctx, digest, &md_len);
    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < md_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    return ss.str();
}

void SHA256HashFile(){
    std::string line;
    std::ifstream inFile("spicypw.dict", std::ios::in);

    std::ofstream outFile("hashesSHA.txt", std::ios::app);

    while(inFile){    
        std::getline(inFile, line);
        outFile << line << ":" << SHA256Hash(line) << std::endl;
    }

    outFile.close();
}

std::string SHA256Hash(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    const unsigned char* data = (const unsigned char*)input.c_str();
    SHA256(data, input.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}




void spicySetUp(){
    std::string line;
    std::ifstream inFile("commonpw.dict", std::ios::in);
    std::ofstream outFile("spicypw.dict", std::ios::app);

    while(inFile){    
        std::getline(inFile, line);
        outFile << line << std::endl;
        outFile << line + "2024" << std::endl;
    }

    outFile.close();
}
//BLIR FÖR STOR FIL PÅ GITHUB
// void spicySetUp(){
//     std::string line;
//     std::ifstream inFile("commonpw.dict", std::ios::in);

//     std::ofstream outFile("spicypw.dict", std::ios::app);

//     while(inFile){    
//         std::getline(inFile, line);
//         outFile << line << std::endl;
//         for(int i = 1940 ; i == 2024 ; i++){
//            outFile << line + std::to_string(i) << std::endl;
//         }
//     }

//     outFile.close();
// }
