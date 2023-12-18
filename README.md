# SharedMemoryChatSystem

## Overview
This repository hosts a Unix-based C program that creates a shared memory communication system. It allows multiple users to send and receive messages in real-time using shared memory, an efficient method for inter-process communication.

## Key Features
- **Shared Memory**: Utilizes shared memory for fast and efficient message passing between processes.
- **Signal Handling**: Implements signal handling for synchronizing messages and handling program updates.
- **User Database**: Manages users with a dedicated structure in shared memory.
- **Interactive CLI**: Offers a command-line interface for sending and receiving messages, adding or removing users.

## Getting Started

### Prerequisites
- Unix-like operating system
- GCC compiler

### Installation
1. Clone the repository:
   ```
   git clone [repository URL]
   ```
2. Navigate to the repository directory and compile the program:
   ```
   gcc user.c -o SharedMemoryChatSystem
   ```

## Usage
  Run the program using:
  ```
  ./SharedMemoryChatSystem
  ```

Follow the on-screen instructions for sending messages, adding users, and managing the communication system.

### Sending a Message
- How to send a message
- Message formatting guidelines

### Receiving Messages
- Instructions on how to receive messages
- Handling multiple messages

## License
This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details.
