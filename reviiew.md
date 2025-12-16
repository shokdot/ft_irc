### PASS wrong password – bug analysis and fixes

#### Summary

- **Bug**: `heap-use-after-free` when a client sends `PASS` with a wrong password.
- **Cause**: A `Client*` was used after the underlying `Client` object had been deleted by the disconnect logic.

---

### Root cause

1. **Wrong PASS flow**
   - `PASS::execute` calls `EventDispatcher::disconnectClient(fd)` on wrong password.
   - `EventDispatcher::disconnectClient` uses `ErrorStrategy`:
     - `ErrorStrategy::handleEvent`:
       - `ChannelManager::partAll(client)`
       - `PollManager::removeFd(fd)`
       - `ClientManager::removeClient(fd)` → `delete client;`
       - `close(fd)`

2. **Use-after-free in message handling**
   - `MsgStrategy::processMsg`:
     - Uses a `Client*` and passes it to `CmdDispatcher::dispatch`.
     - Some commands (e.g. `PASS` wrong password) can delete that `Client` via the path above.
   - Previously, the same `Client*` could be reused for subsequent iterations / commands after it was deleted, leading to:
     - `Client::getClientFd()` being called on freed memory.

3. **Event loop & pollfd list**
   - `EventDispatcher::handleEvents` iterates over the `PollManager`’s `pollfd` list.
   - During handling, `ErrorStrategy` removes FDs from `PollManager`, which risks iterator invalidation if the dispatcher iterates over the live container.

---

### Fixes implemented

#### 1. Safe iteration over poll fds

- **Files**: `EventDispatcher.cpp`, `PollManager.hpp`, `PollManager.cpp`
- **Changes**:
  - `PollManager::getPollFds()` now returns a **copy** of the internal `std::vector<struct pollfd>` and is marked `const`:
    - Avoids exposing internal storage by non-const reference.
  - `EventDispatcher::handleEvents()` now:
    - Copies the list: `std::vector<struct pollfd> fds = _pollManager.getPollFds();`
    - Iterates over this copy while strategies may remove FDs from the real `PollManager`.
- **Effect**:
  - Removing FDs during error handling (`disconnectClient` / `disconnectAllClients`) no longer invalidates the iterator used in the event loop.

#### 2. Prevent dispatch on deleted clients

- **File**: `MsgStrategy.cpp`
- **Original risk**:
  - A command like `PASS` could delete the `Client` during `_dispatcher.dispatch(client, line, server)`.
  - Subsequent use of the same `Client*` (or another loop iteration) could then access freed memory.
- **Current logic**:
  - Fetch `Client*` before processing:
    - `Client *client = clientManager.getClientByFd(fd);`
  - After each dispatch:
    - Re-check: `if (!clientManager.getClientByFd(fd)) { _sockBuffer.erase(fd); return; }`
  - This ensures:
    - If a command disconnects the client, the buffer is cleared and processing for that fd stops immediately.
- **Recommended refinement (optional)**:
  - Re-fetch `client` inside the loop before each command and early-exit if `client` is already gone before processing pending lines.

---

### Behavior after fix

- **Wrong PASS**:
  - Client receives `ERR_PASSWDMISMATCH`.
  - `EventDispatcher::disconnectClient` fully cleans up the client.
  - Any pending data for that fd in `_sockBuffer` is dropped.
  - No further commands are dispatched for that deleted client.
  - Sanitizer should no longer report `heap-use-after-free` in `Client::getClientFd()`.

---

### Notes / Follow-ups

- Consider systematically checking for a valid `Client*` in any place where commands can disconnect clients mid-processing.
- Keep `PollManager`’s internal `_fds` encapsulated (as it is now) to avoid similar iterator invalidation issues elsewhere.


