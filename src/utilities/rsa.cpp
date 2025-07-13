//
// Created by backo on 04/07/25.
//
#include "utilities/rsa.h"
std::unique_ptr<std::vector<unsigned char>> dpp_structures::sign_sha256_rsa(const std::string& base, const std::string& private_key) {
    EVP_MD_CTX* evp_context = EVP_MD_CTX_new();

    BIO* key_bio = BIO_new_mem_buf((void*) private_key.c_str(), -1);
    if (!key_bio) {
        return nullptr;
    }

    EVP_PKEY* evp_private_key = PEM_read_bio_PrivateKey(key_bio, nullptr, nullptr, nullptr);
    BIO_free(key_bio);

    DIGEST_STEP(EVP_DigestSignInit(evp_context, nullptr, EVP_sha256(), nullptr, evp_private_key))
    DIGEST_STEP(EVP_DigestSignUpdate(evp_context, base.c_str(), base.length()))

    size_t encoded_message_length;
    DIGEST_STEP(EVP_DigestSignFinal(evp_context, nullptr, &encoded_message_length));

    auto encoded_message = std::make_unique<std::vector<unsigned char>>(encoded_message_length);

    DIGEST_STEP(EVP_DigestSignFinal(evp_context, encoded_message->data(), &encoded_message_length))

    EVP_MD_CTX_free(evp_context);
    EVP_PKEY_free(evp_private_key);

    return encoded_message;

}