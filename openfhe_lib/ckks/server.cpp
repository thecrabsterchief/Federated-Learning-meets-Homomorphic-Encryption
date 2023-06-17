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
    location for storing keys
    === IMPORTANT ===
    If running locally, you may want to replace the "hardcoded" `DATAFOLDER` with
    the `DATAFOLDER` location below which gets the current working directory
*/
const std::string DATAFOLDER       = "data/ckks";
const std::string ccLocation       = "/crypto_context.txt";
const std::string pubKeyLocation   = "/public_key.txt";         // Pub key
const std::string multKeyLocation  = "/mult_key.txt";

CryptoContext<DCRTPoly> CC;
PublicKey<DCRTPoly> serverPubKey;
std::vector< Ciphertext<DCRTPoly> > enc_weights;

void load_crypto_context(CryptoContext<DCRTPoly>& CC) {
    /*
        Load Crypto Context from file
    */
    CC->ClearEvalMultKeys();
    CC->ClearEvalAutomorphismKeys();

    lbcrypto::CryptoContextFactory<lbcrypto::DCRTPoly>::ReleaseAllContexts();
    if (!Serial::DeserializeFromFile(DATAFOLDER + ccLocation, CC, SerType::BINARY)) {
        std::cerr << "I cannot read serialized data from: " << DATAFOLDER << ccLocation << std::endl;
        std::exit(1);
    }
}

void load_public_key(PublicKey<DCRTPoly>& pubKey) {
    /*
        Load Public Key
    */
    if (!Serial::DeserializeFromFile(DATAFOLDER + pubKeyLocation, pubKey, SerType::BINARY)) {
        std::cerr << "I cannot read serialized data from: " << DATAFOLDER << pubKeyLocation << std::endl;
        std::exit(1);
    }
}

void load_mult_key() {
    /*
        Load Mult Key
    */
    std::ifstream multKeyIStream(DATAFOLDER + multKeyLocation, std::ios::in | std::ios::binary);
    if (!multKeyIStream.is_open()) {
        std::cerr << "Cannot read serialization from " << DATAFOLDER + multKeyLocation << std::endl;
        std::exit(1);
    }
    if (!CC->DeserializeEvalMultKey(multKeyIStream, SerType::BINARY)) {
        std::cerr << "Could not deserialize eval mult key file" << std::endl;
        std::exit(1);
    }
}


void load_client_encrypted_weights() {
    /*
        Load the encrypted weights of clients from files then save it to
        global `std::vector< Ciphertext<DCRTPoly> > enc_weights;`
    */
    std::vector< std::string > paths = {
        "/enc_weight_client1.txt",
        "/enc_weight_client2.txt",
        "/enc_weight_client3.txt",
        "/enc_weight_client4.txt"
    };

    for (auto path: paths) {
        Ciphertext<DCRTPoly> enc;
        Serial::DeserializeFromFile(DATAFOLDER + path, enc, SerType::BINARY);
        enc_weights.push_back(enc);
    }
}

void aggregator(std::vector< Ciphertext<DCRTPoly> >encrypted_weights, double n) {
    /*
        This function peform weights aggregation (federated learning concept)
    */
    auto sum = encrypted_weights[0];
    for(int i=1; i<encrypted_weights.size(); ++i)
        sum = CC->EvalAdd(sum, encrypted_weights[i]);

    auto result = CC->EvalMult(sum, 1.0/n);
    
    // saving server result
    if (!Serial::SerializeToFile(DATAFOLDER + "/enc_aggregator_weight_server.txt", result, SerType::BINARY)) {
        std::cerr << " Error writing ciphertext" << std::endl;
    }
}

int main(int argc, char* argv[]) {

    // Load crypto context from files
    load_crypto_context(CC);
    load_public_key(serverPubKey);
    load_mult_key();
    
    // load clients's encrypted weights
    load_client_encrypted_weights();
    
    // perform aggregation
    double n_hospitals = 4;
    aggregator(enc_weights, n_hospitals);
    return 0;
}