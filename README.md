# Grep
Implementation of grep following course from: https://app.codecrafters.io/courses/grep/overview.

## Overview

This project implements a C++ program with various components for matching and tokenizing input. The core logic is implemented in different modules that interact with each other through a well-defined API.

### Project Structure

```
.
├── include/
│   ├── Matcher.hpp
│   ├── tokenizer.hpp
│   ├── tokens.hpp
│   └── utils.hpp
├── src/
│   ├── Matcher.cpp
│   ├── Server.cpp
│   ├── tokenizer.cpp
│   └── tokens.cpp
├── build/
├── test_grep.sh
├── Makefile
└── README.md
```

- **include/**: Header files defining classes and methods.
    - `Matcher.hpp`: Handles the matching logic.
    - `tokenizer.hpp`: Responsible for tokenizing input.
    - `tokens.hpp`: Defines the different token types.
    - `utils.hpp`: Utility functions used across the project.

- **src/**: C++ source files implementing the functionality.
    - `Matcher.cpp`: Implements the matching logic.
    - `Server.cpp`: Entry point for the server, includes `Matcher.hpp`.
    - `tokenizer.cpp`: Implements tokenization logic.
    - `tokens.cpp`: Implements token types and behaviors.

- **build/**: Directory where object files (`.o`) are generated after compilation.

- **test_grep.sh**: Test script for validating the program’s functionality.

## Prerequisites

- **C++20** or newer
- **g++** (or compatible compiler)

## Compilation

To compile the project, run the following command in the root directory:

```bash
make
```

This will compile the source files and generate the `server` binary.

## Running the Program

After compiling, you can run the program using:

```bash
./server
```

## Testing

The project includes a `test_grep.sh` script for testing the functionality of the program. This script runs a set of tests to verify that the program behaves correctly for different input scenarios.

These tests were adapted from [Codecrafters GREP Tester](https://github.com/codecrafters-io/grep-tester), a testing suite that evaluates GREP-like implementations. You can run the tests using the following command:

```bash
./test_grep.sh
```

The script will automatically run the test cases and output the results, indicating whether the program passed or failed each test.


## Cleaning Up

To clean up the build files (object files and binary), run:

```bash
make clean
```

This will remove the `build/` directory and the `server` binary.
