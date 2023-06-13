from openFHE import *

def demo():
    print("""
    /*
    Demo openFHE library: This is a simple python wrapper of openFHE library
    using CKKS scheme
    > link: https://github.com/openfheorg/openfhe-development/tree/main
    */
    """)
    print("=== This is demostration of calculating (w1 + w2 + w3)/3 using openFHE ===")
    w1 = [0.11231323, -4.112312394, 3.012444, -0.42341, 1.07234235, 1.123]
    w2 = [-2.323133, 7.134434, 0.010234, 13.052342342, 0.02405, -2.322]
    w3 = [-1.55135, -1.983334, 2.3234231, -8.424441, 0.033245, -3.33]
    n = len(w1)
    
    print("w1 = " + str(w1))
    print("w2 = " + str(w2))
    print("w3 = " + str(w3))

    print("\n[+] Homo encryption by clients...")
    print("> Encrypt w1 -> ./data/enc_weight_client1.txt")
    encrypt_weights(w1, "/enc_weight_client1.txt")
    print("> Encrypt w2 -> ./data/enc_weight_client2.txt")
    encrypt_weights(w2, "/enc_weight_client2.txt")
    print("> Encrypt w3 -> ./data/enc_weight_client3.txt")
    encrypt_weights(w3, "/enc_weight_client3.txt")

    print("\n[+] Homo add + mult by server...")
    print("> server calculate -> ./data/enc_aggregator_weight_server.txt")
    aggregator()

    print("\n[+] Homo decryption by clients...")

    print("\n[+] Compare final result: ")
    predict = [(a + b + c)/3 for a,b,c in zip(w1, w2, w3)]
    print("> True value: " + str(predict))
    print("> Use homo: " + str(decrypt_weights("/enc_aggregator_weight_server.txt")[:n]))
    
    
if __name__ == '__main__':
    demo()