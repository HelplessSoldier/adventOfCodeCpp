#include "openssl/evp.h"
#include <cstdio>
#include <iostream>
#include <string>

// g++ 04_idealStockingStuffer.cpp -o 04_idealStockingStuffer -lssl -lcrypto

bool isAnswer(const std::string &hashedString) {
  int consecutiveZerosCount = 5;
  for (int i = 0; i < consecutiveZerosCount; ++i) {
    if (hashedString[i] != '0') {
      return false;
    }
  }
  return true;
}

std::string md5(const std::string &content) {
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  const EVP_MD *md = EVP_md5();
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned int md_len;
  std::string output;

  EVP_DigestInit_ex2(context, md, NULL);
  EVP_DigestUpdate(context, content.c_str(), content.length());
  EVP_DigestFinal_ex(context, md_value, &md_len);
  EVP_MD_CTX_free(context);

  output.resize(md_len * 2);
  for (unsigned int i = 0; i < md_len; ++i)
    std::sprintf(&output[i * 2], "%02x", md_value[i]);
  return output;
}

int main() {
  std::string input = "yzbqklnj";

  int endNum = 0;
  std::string hashedString = "9999999";
  std::string checkStr;

  while (!isAnswer(hashedString)) {
    checkStr = input + std::to_string(endNum);
    hashedString = md5(checkStr);
    ++endNum;
  }

  std::cout << "input: " << checkStr << " produced: " << hashedString
            << " using end num of: " << endNum - 1 << std::endl;

  return 0;
}
