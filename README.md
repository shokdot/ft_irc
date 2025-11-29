# ft_irc

**ft_irc** is a modular IRC server implementation written in modern C++.  
It follows the core IRC protocol and supports essential client and channel operations.  
The architecture is built around dispatcher and strategy patterns, making the server flexible, maintainable, and easy to extend.

---

## 🚀 Features

### **Supported IRC Commands**
The server currently implements the following IRC commands:

- `PASS`
- `NICK`
- `USER`
- `JOIN`
- `PART`
- `KICK`
- `INVITE`
- `MODE`
- `TOPIC`
- `PING`
- `QUIT`
- `CAP`

Each command is isolated into its own class under `includes/Commands/`.

---

## 🧱 Architecture Overview

The project is decomposed into multiple modules, each responsible for a specific part of the IRC server.

### **Command System**
Located in:  
`ACommand/`, `Commands/`, `CmdRegistry/`, `CmdDispatcher/`

- Each IRC command is implemented using an object-oriented structure.
- Commands are registered globally and dispatched dynamically.
- Ensures clean separation between parsing and execution.

### **Client & Channel Management**
Located in:  
`Client/`, `ClientManager/`, `Channel/`, `ChannelManager/`

Handles:

- Client state tracking  
- Nickname management  
- Channels, membership, topic, and permissions  
- Modes and access levels  

### **Event & Input Processing**
Located in:  
`EventDispatcher/`, `IEventStrategy/`, `MsgStrategy/`, `ErrorStrategy`, `InputValidator/`

Responsibilities:

- Validating input format  
- Parsing user messages  
- Routing events to the correct handlers  
- Error handling (numeric replies, validation errors)  

### **Networking & Server Loop**
Located in:  
`ircServer/`, `PollManager/`, `AcceptStrategy/`

- Uses `poll()` for socket multiplexing.
- Connection acceptance and client message handling are implemented through strategy objects.

### **Replies**
Located in:  
`Replies/`

- Contains helpers for returning numeric IRC codes and formatted replies.

---

## 🎯 Design Patterns Used

### **Strategy Pattern**
Used in:

- `AcceptStrategy/`
- `MsgStrategy/`
- `ErrorStrategy/`
- `IEventStrategy/`

Allows swapping behaviors at runtime without modifying core logic.

---

### **Command Pattern**
Applied to every IRC command (`JOIN`, `KICK`, `PING`, etc.).  
Commands inherit from a base class and implement their own execution logic.

---

### **Dispatcher Pattern**
Used by:

- `CmdDispatcher/`
- `CmdRegistry/`

Incoming messages → parsed → mapped to the correct command class → executed.

---

### **Factory / Registry Pattern**
`CmdRegistry/` works as a command factory:

- Stores mappings between command names and constructors
- Makes adding new commands trivial

---

### **Observer / Event Pattern** (lightweight)
Used in:

- `EventDispatcher/`

Enables event-based processing of client actions.

---

### **Manager Pattern**
Seen in:

- `ClientManager/`
- `ChannelManager/`
- `PollManager/`

Centralizes state management and operations.

---

## ▶️ Running the Server

```bash
git clone https://github.com/shokdot/ft_irc
cd ft_irc
make
./ircserv <port> <password>
```

## 📌 Project Goal

The goal of ft_irc is to build a functioning IRC server that demonstrates:

- Network programming
- Event-driven architectures
- Command parsing
- OOP design with multiple patterns
- Real-time server-client communication
