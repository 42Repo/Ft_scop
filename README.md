
## Instructions de Build

1. **Clone the Repo**

```bash
git clone https://github.com/42Repo/Ft_scop
cd Ft_scop
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
