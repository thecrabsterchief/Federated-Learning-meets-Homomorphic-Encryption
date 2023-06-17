#define PROFILE

#include <iomanip>
#include <tuple>
#include <unistd.h>

#include "openfhe.h"

// header files needed for serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"
using namespace lbcrypto;


/*
    constant parameters
*/
const int multDepth    = 5;
const int scaleModSize = 40;
const usint batchSize  = 32;

/*
    location for storing keys
    === IMPORTANT ===
    If running locally, you may want to replace the "hardcoded" `DATAFOLDER` with
    the `DATAFOLDER` location below which gets the current working directory
*/
const std::string DATAFOLDER       = "data/ckks";
const std::string ccLocation       = "/crypto_context.txt";
const std::string pubKeyLocation   = "/public_key.txt";         // Pub key
const std::string priKeyLocation   = "/private_key.txt";        // Pri key
const std::string multKeyLocation  = "/mult_key.txt";

CryptoContext<DCRTPoly> CC;
KeyPair<DCRTPoly> KP;

void set_PKKS_Context(CryptoContext<DCRTPoly>& CC) {
    /*
        initialize parameters
    */
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetBatchSize(batchSize);
    
    /*
        initialize crypto context
    */
    CC = GenCryptoContext(parameters);
    CC->Enable(PKE);
    CC->Enable(KEYSWITCH);
    CC->Enable(LEVELEDSHE);
}

void save_crypto_context() {
    // saving CryptoContext
    if (!Serial::SerializeToFile(DATAFOLDER + ccLocation, CC, SerType::BINARY)) {
        std::cerr << "Error writing serialization of the crypto context to "
                     "crypto_context.txt"
                << std::endl;
        std::exit(1);
    }
}

void save_public_key() {
    // saving Public Key
    if (!Serial::SerializeToFile(DATAFOLDER + pubKeyLocation, KP.publicKey, SerType::BINARY)) {
        std::cerr << "Exception writing public key to public_key.txt" << std::endl;
        std::exit(1);
    }
}

void save_private_key() {
    // saving Private Key
    if (!Serial::SerializeToFile(DATAFOLDER + priKeyLocation, KP.secretKey, SerType::BINARY)) {
        std::cerr << "Exception writing public key to private_key.txt" << std::endl;
        std::exit(1);
    }
}

void save_mult_key() {
    // saving mult key
    std::ofstream multKeyFile(DATAFOLDER + multKeyLocation, std::ios::out | std::ios::binary);
    if (multKeyFile.is_open()) {
        if (!CC->SerializeEvalMultKey(multKeyFile, SerType::BINARY)) {
            std::cerr << "Error writing eval mult keys" << std::endl;
            std::exit(1);
        }
        multKeyFile.close();
    }
    else {
        std::cerr << "Error serializing EvalMult keys" << std::endl;
        std::exit(1);
    }
}

int main() {
    
    // setup CKKS Crypto Context
    set_PKKS_Context(CC);

    // generate keypairs
    KP = CC->KeyGen();

    // generate Mult Keys/ Relinearization keys
    CC->EvalMultKeyGen(KP.secretKey);

    // saving keys
    save_crypto_context();
    save_public_key();
    save_private_key();
    save_mult_key();
    return 0;
}