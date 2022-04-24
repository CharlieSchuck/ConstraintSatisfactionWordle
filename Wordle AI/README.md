# Wordle AI

## Authors
* Charles Schuck
* Eric Grandizio

## About

This folder contains the code for our Wordle-playing AI Program.

When the program is run, it will display a "help message" detailing how to use it.

The AI plays a simulated version of Wordle until it guesses the correct word.

It does so by maintaining a list of possible words, and removing said words from the list based on the feedback it gets for its guesses.

Even so, in order to actually win the game, it must guess right in 6 turns or less.

On any given turn, it could have tens of hundreds of possible words to guess from.

As such, the AI employs some heuristics to choose words it thinks will give it the most information.

## Compiling on Linux

The AI Program expects a very particular file structure in order to work properly.
```
WordleAI/
├── AI Program/
│   ├── src/
│   │   └── ...
│   ├── Tests/
│   │   └── ...
│   └── Program.exe
└── Dictionaries/
    ├── scrabble-dict.txt
    ├── wordle-answers.txt
    └── wordle-guesses.txt
```
Make sure to compile and run the AI Program from the `AI Program/` directory.

`src/` contains the source code for the AI.

The AI Program will write the results of any tests that are run to the `Tests/` directory.
If the directory does not exist, the program will terminate upon trying 

### Important Note

This project uses the standard `<execution>` header from C++17 in order to use Multithreading to speed up the Benchmarking code.

The header is available in GCC versions 9 and up, but requires some additional setup (compared to Visual Studio) in order to use it.

As such, we have provided options for compiling With or Without the multithreading.

These methods were tested using **g++ 9.4.0** on an **Ubuntu 20.04** virtual machine.

## Multi-Threaded

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

## Single-Threaded

When compiling, make sure to use the `-std=c++17` flag and use `-D NO_MULTITHREADING` to define a macro that disables multithreading.

#### Example:
```
g++ src/*.cpp -std=c++17 -O3 -D NO_MULTITHREADING -o WordleAI
```
