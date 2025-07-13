//
// Created by backo on 04/07/25.
//

#ifndef DPP_STRUCTURES_RSA_H
#define DPP_STRUCTURES_RSA_H

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>
#include <vector>
#include <memory>

#define DIGEST_STEP(expr) if(expr <= 0) {return nullptr;}

namespace dpp_structures {


    std::unique_ptr<std::vector<unsigned char>> sign_sha256_rsa(const std::string& base, const std::string& private_key);
}

#endif //DPP_STRUCTURES_RSA_H
