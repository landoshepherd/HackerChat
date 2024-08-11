# HackerChat

HackerChat is messaging platform the nostalgic computer geek. It is a simple chat client that allows user to chat with other users all within the terminal.    
  
  ![Static Badge](https://img.shields.io/badge/Project%20Stage-Experimental-yellow)

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
  ### HackerChatServer
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
  ### HackerChatClient
- Navigate to HackerChatClient directory
```bash
$ cd HackerChat/HackerChatClient
```
  - Run CMake to create Makefile
  - Note: External includes and libraries should be at this location. This is where CMake will find all of the files necessary to build the makefile.
  - The "build" directory is where the cmake build artifacts, including the makefile and eventually the executable, will be located.
```bash
$ cmake -DEXTERNAL_FILES_DIR=/path/to/external/files -B build/ ./
```
- Define HACKERCHAT_CLIENT_CONFIG_DIR environment variable
- To run the HackerChatClient successfully, it needs to know where the HCClientConfig.json file is located. This is where the websocket and all other configurations will be stored.
```bash
$ export HACKERCHAT_CLIENT_CONFIG_DIR=/path/to/config/dir
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
$ cd HackerChat/HackerChatServer/build
$ ./HackerChatServer
```
- In the other terminal navigate to HackerChatClient executable and run program
```bash
$ cd HackerChat/HackerChatClient/build
$ ./HackerChatClient
```
- In the HackerChatClient terminal, type in a message and press enter.
- ![Screenshot 2024-03-24 at 9 38 01 PM](https://github.com/landoshepherd/HackerChat/assets/30390801/795b55ce-9943-4281-ad32-b4b9f8b1db7b)

- The same message should appear in the HackerChatServer terminal and echoed back to the HackerChatClient terminal.
- ![Screenshot 2024-03-24 at 9 34 52 PM](https://github.com/landoshepherd/HackerChat/assets/30390801/810aadec-e4eb-4487-9275-e9a1efa64986)
- ![Screenshot 2024-03-24 at 9 34 35 PM](https://github.com/landoshepherd/HackerChat/assets/30390801/b0406cc5-992e-421a-8de6-b3e4db659af3)

## Features
- TBD
## Contributing
- TBD
## License
- MIT License
