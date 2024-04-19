# Hamming-Code-Implementation

This project is a task on course "Fundamentals of telecommunication technologies" by A.A. Kureev (is also a Project on Computer Technologies course [_4th semester_] at MIPT). The task was to implement encode/decode utilties for single-bit error correcting.

## Table of Contents
1. [Requirements](#requirements)
2. [Building](#building)
3. [Usage](#usage)
     * [Examples](#c-examples)
4. [TODO List](todo-list)
5. [References](#references)

## Requirements

The following application has to be installed:
- CMake of version 3.15 (or higher)

## Building

**Step 1:** Clone this repository
```bash
git clone git@github.com:d-r-tikhonov/Hamming-Code-Implementation.git
cd Hamming-Code-Implementation
```

**Step 2:** Create ***build*** folder.
```bash
mkdir build
cd build
```

**Step 3:** Use CMake:
```bash
cmake ..
cmake --build . --config <Debug|Release>
``` 

**Step 4:** Build the project and run the program:
```bash
make
./bin/HammingCode
```

## Usage

### Examples

## TODO List

## References
Further information about the Hamming encoding algorithm employed within `hamming-code-implementation` can be found in the following resources:
  * [3Blue1Brown's "Hamming codes and error correction"](https://www.youtube.com/watch?v=X8jsijhllIA)
  * [tutorialspoint's "Error Correcting Codes - Hamming codes"](https://www.tutorialspoint.com/error-correcting-codes-hamming-codes)
  * [Daniel Antrim "Hamming-codec"](https://github.com/dantrim/hamming-codec)