# Wordle AI

[ Insert Description Here ]

## Compiling on Linux

This project uses the standard `<execution>` header from C++17 in order to use Multithreading to speed up the Benchmarking code.

The header is available in GCC versions 9 and up, but requires some additional setup (compared to Visual Studio) in order to use it.

First, the following commands will need to be run:
```
$ sudo apt update
$ sudo apt install libtbb-dev
```

Then, when compiling, make sure to use the `-std=c++17` and `-ltbb` flags.

#### Example:
```
g++ src/*.cpp -std=c++17 -O3 -ltbb -o WordleAI
```

#### Source:
https://stackoverflow.com/questions/57084789/how-to-use-execution-library-in-c17)

This method was tested using **g++ 9.4.0** on an **Ubuntu 20.04** virtual machine.
