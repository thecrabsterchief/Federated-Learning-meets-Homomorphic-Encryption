import subprocess
import pathlib

# get current directory of this file
cwd = pathlib.Path(__file__).parent.resolve()
WW = 10**6

def generate_keys():
    """ Just a python wrapper to run ./build/key_gen. After running this files, it will
        create `./data` folders that holds:
        - `crypto_context.txt` : contain BGV CryptoContext object
        - `public_key.txt`     : contain BGV public key
        - `private_key.txt`    : contain BGV private key
        - `mult_key.txt`       : contain BGV multiplication key
    """
    subprocess.run(f"{cwd}/build/key_generation")

def encrypt_weights(weights, output_filename):
    """ This function will be used by clients to encrypt local model's weights and send its to server.
        - weights         : vector of model's weights (float numbers)
        - output_filename : store the output of encrypted_weights (as `Ciphertext<DCRTPoly>` object) to `output_filename`.
    """
    subprocess.run([f"{cwd}/build/client", output_filename, "@".join([str(int(w * WW)) for w in weights])])

def decrypt_weights(cipher_file) -> list[float]:
    """ This function will be used by clients to decrypt aggregated global model's weights.
        - cipher_file     : contain aggregated_weights compute using homomorphic operations by server
    """
    
    stdout = subprocess.run([f"{cwd}/build/client", cipher_file], capture_output=True).stdout.decode()
    return [float(int(w) / (4 * WW)) for w in stdout.split("@")]

def aggregator():
    """ This function will be used by server to calculate aggregated global model's weights by applying
        homomorphic encryption to encrypted local model's weights of clients
    """
    subprocess.run([f"{cwd}/build/server"])
