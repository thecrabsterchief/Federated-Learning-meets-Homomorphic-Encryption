import subprocess
import os
import sys

os.chdir(sys.path[0])

def generate_keys():
    """ Just a python wrapper to run ./build/key_gen. After running this files, it will
        create `./data` folders that holds:
        - `crypto_context.txt` : contain CKKS CryptoContext object
        - `public_key.txt`     : contain CKKS public key
        - `private_key.txt`    : contain CKKS private key
        - `mult_key.txt`       : contain CKKS multiplication key
    """
    subprocess.run("build/key_generation")

def encrypt_weights(weights, output_filename):
    """ This function will be used by clients to encrypt local model's weights and send its to server.
        - weights         : vector of model's weights (float numbers)
        - output_filename : store the output of encrypted_weights (as `Ciphertext<DCRTPoly>` object) to `output_filename`.
    """
    subprocess.run(["build/client", output_filename, "@".join([str(w) for w in weights])])

def decrypt_weights(cipher_file) -> list[float]:
    """ This function will be used by clients to decrypt aggregated global model's weights.
        - cipher_file     : contain aggregated_weights compute using homomorphic operations by server
    """
    
    stdout = subprocess.run(["./build/client", cipher_file], capture_output=True).stdout.decode()
    return [float(w) for w in stdout.split("@")]

def aggregator():
    """ This function will be used by server to calculate aggregated global model's weights by applying
        homomorphic encryption to encrypted local model's weights of clients
    """
    subprocess.run(["build/server"])