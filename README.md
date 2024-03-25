# HackerChat

HackerChat is messaging platform the nostalgic computer geek. It is a simple chat client that allows user to chat with other users all within the terminal.

## Table of Contents

- [Dependencies](#dependecies)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Contributing](#contributing)
- [License](#license)

## Dependencies
  - Requires Linux or Unix like operating systems
    - Will port to Windows in the future
  - C++17 Compiler
  - Boost (Min. Version 1.83.0) ASIO and Beast Libraries
    - ASIO and Beast libraries and installation instructions can be found [here](https://www.boost.org/)
  - [Rapid Json](https://rapidjson.org/) (Min. Version 1.0)
  - [PugiXml](https://pugixml.org/) (Min. Version 1.14)
  - [CMake](https://cmake.org/download/) (Min. Version 3.26)
## Installation
  - Clone the repo
```bash
$ git clone https://github.com/landoshepherd/HackerChat.git
```
  - Navigate to HackerChatServer directory
```bash
$ cd HackerChat/HackerChatServer
```
  - Run CMake to create Makefile
```bash
$ cmake -B build/ ./
```
  - Navigate to build directory and run make command to build HackerChatServer executable
```bash
$ cd HackerChat/HackerChatServer/build
$ make
```
- Navigate to HackerChatClient directory
```bash
$ cd HackerChat/HackerChatClient
```
  - Run CMake to create Makefile
```bash
$ cmake -B build/ ./
```
  - Navigate to build directory and run make command to build HackerChatClient executable
```bash
$ cd HackerChat/HackerChatClient/build
$ make
```
## Usage
- Open two separate terminals
- In one terminal navigate to HackerChatServer executable and run program
```bash
$ cd HackerChat/HackerChatServer/build/bin
$ ./HackerChatServer
```
- In the other terminal navigate to HackerChatClient executable and run program
```bash
$ cd HackerChat/HackerChatClient/build/bin
$ ./HackerChatClient
```
- In the HackerChatClient terminal, type in a message and press enter.
- The same message should appear in the HackerChatServer terminal and echoed back to the HackerChatClient terminal.
## Features
- TBD
## Contributing
- TBD
## License
- Comming soon
