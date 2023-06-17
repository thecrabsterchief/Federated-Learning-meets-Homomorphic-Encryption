#include <iomanip>
#include <tuple>
#include <unistd.h>

#include "openfhe.h"

// header files needed for serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "scheme/bfvrns/bfvrns-ser.h"

using namespace lbcrypto;

/////////////////////////////////////////////////////////////////
// NOTE:
// If running locally, you may want to replace the "hardcoded" DATAFOLDER with
// the DATAFOLDER location below which gets the current working directory
/////////////////////////////////////////////////////////////////
// char buff[1024];
// std::string DATAFOLDER = std::string(getcwd(buff, 1024));

// Save-Load locations for keys
const std::string DATAFOLDER       = "data/bfv";
const std::string ccLocation       = "/crypto_context.txt";
const std::string pubKeyLocation   = "/public_key.txt";         // Pub key
const std::string priKeyLocation   = "/private_key.txt";        // Pri key
const std::string multKeyLocation  = "/mult_key.txt";


CryptoContext<DCRTPoly> CC;
PublicKey<DCRTPoly> clientPubKey;
PrivateKey<DCRTPoly> clientPriKey;

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

void load_priv_key(PrivateKey<DCRTPoly>& priKey) {
    /*
        Load Private Key
    */
    if (!Serial::DeserializeFromFile(DATAFOLDER + priKeyLocation, priKey, SerType::BINARY)) {
        std::cerr << "I cannot read serialized data from: " << DATAFOLDER << priKeyLocation << std::endl;
        std::exit(1);
    }
}

void encrypt(std::string filename, std::vector<int64_t> vec) {
    /*
        This function will encrypt `vec` then put the result to `filename`  
    */
    Plaintext plain = CC->MakePackedPlaintext(vec);
    auto ct = CC->Encrypt(clientPubKey, plain);

    // saving ciphertext
    if (!Serial::SerializeToFile(DATAFOLDER + filename, ct, SerType::BINARY)) {
        std::cerr << " Error writing ciphertext" << std::endl;
    }
}

void decrypt(std::string filename, std::vector<int64_t>& out) {
    /*
        This function will load the ciphertext from `filename`, decrypt it and put the
        result to vector out
    */
    
    Ciphertext<DCRTPoly> enc;
    Serial::DeserializeFromFile(DATAFOLDER + filename, enc, SerType::BINARY);

    Plaintext plain;
    CC->Decrypt(clientPriKey, enc, &plain);
    plain->SetLength(32);
    out = plain->GetPackedValue();
}

void parse_int64_vector(std::string input, std::vector<int64_t>& output) {
    /*
        This function will parse the string to vector<int64_t>
    */
    // Use stringstream to split the string
    std::stringstream ss(input);
    int64_t num;

    while (ss >> num) {
        output.emplace_back(num);

        // Skip whitespace characters
        while (ss.peek() == '@') {
            ss.ignore();
        }
    }
}


int main(int argc, char* argv[]) {

    // setup Crypto Context
    load_crypto_context(CC);
    load_public_key(clientPubKey);
    load_priv_key(clientPriKey);

    std::vector<int64_t> inp, out;
    
    if (argc == 3) {
        // weights vector
        parse_int64_vector(argv[2], inp);

        // filename
        encrypt(argv[1], inp);
    }

    else {
        decrypt(argv[1], out);
        for (int i=0; i<31; ++i) {
            std::cout << out[i] << '@';
        }
        std::cout << out[31];
    }
    return 0;
}