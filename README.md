
## Instructions de Build

1. **Clone the Repo**

```bash
git clone https://github.com/votre-utilisateur/projet-xyz.git
cd projet-xyz
```
2.  **Building The Project**
```bash
mkdir build
cd build

# For GCC
export CC=gcc
export CXX=g++

# For Clang
# export CC=clang
# export CXX=clang++

# Standard configuration
cmake ..

# Or with options
# cmake -DCMAKE_BUILD_TYPE=Debug ..

# Compile
make
```
