# OpenFHE - Open-Source Fully Homomorphic Encryption Library

## 1. Install library

Follow [this](https://openfhe-development.readthedocs.io/en/latest/sphinx_rsts/intro/installation/linux.html) instructions

## 2. How to include openFHE library to our project?

Follow this [turorial](https://openfhe-development.readthedocs.io/en/latest/sphinx_rsts/intro/building_user_applications.html?fbclid=IwAR0KfAp9A_UsAC98hlfN3g_62eOHVWb9892uvZVrCR6JnSTlNPv586XDsNM)

For our project, there are several main files:

- `key_generation.cpp`: generate some keys of CKKS crypto context using openFHE library.
- `client.cpp`: define some useful functions which will be used by clients in encrypted-learning models.
- `server.cpp`: define some useful functions which will be used by server in encrypted-learning models.
- `openFHE.py`: just a python wrapper for convenient.

## 3. Demo CKKS scheme using openFHE

Simple demo:

```python
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
```

Result:

```bash
$ python demo.py

    /*
    Demo openFHE library: This is a simple python wrapper of openFHE library
    using CKKS scheme
    > link: https://github.com/openfheorg/openfhe-development/tree/main
    */
    
=== This is demostration of calculating (w1 + w2 + w3)/3 using openFHE ===
w1 = [0.11231323, -4.112312394, 3.012444, -0.42341, 1.07234235, 1.123]
w2 = [-2.323133, 7.134434, 0.010234, 13.052342342, 0.02405, -2.322]
w3 = [-1.55135, -1.983334, 2.3234231, -8.424441, 0.033245, -3.33]

[+] Homo encryption by clients...
> Encrypt w1 -> ./data/enc_weight_client1.txt
> Encrypt w2 -> ./data/enc_weight_client2.txt
> Encrypt w3 -> ./data/enc_weight_client3.txt

[+] Homo add + mult by server...
> server calculate -> ./data/enc_aggregator_weight_server.txt

[+] Homo decryption by clients...

[+] Compare final result: 
> True value: [-1.25405659, 0.3462625353333333, 1.7820337000000002, 1.4014971139999997, 0.3765457833333333, -1.5096666666666667]
> Use homo: [-1.2540565900427696, 0.3462625354233809, 1.7820336999534137, 1.401497113994267, 0.37654578323198307, -1.5096666666273513]

$
```