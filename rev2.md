## ft_irc – Codebase Review (rev2)

This document provides a holistic review of the current `ft_irc` codebase, focusing on **architecture**, **correctness**, **robustness**, **performance**, and **code style/maintainability**. It is based on reading the main modules (`ircServer`, `EventDispatcher`, strategies, managers, core commands, validation, and utilities), not on runtime tests.

---

## 1. High-level Architecture

- **Overall design**
  - The project follows a **clean modular architecture**: networking (`IRCServer`, `PollManager`, strategies), state management (`ClientManager`, `ChannelManager`, `Channel`, `Client`), and command handling (`CmdParser`, `CmdDispatcher`, `CmdRegistry`, `ACommand` + concrete commands).
  - The use of **Strategy**, **Command**, and **Dispatcher/Registry** patterns is appropriate and well-separated:
    - `EventDispatcher` + `IEventStrategy` with `AcceptStrategy`, `MsgStrategy`, `ErrorStrategy`.
    - `ACommand` + concrete command classes, wired via `CmdRegistry`.
  - **Pros**
    - Easy to add new commands (one header, one cpp, register in `CmdRegistry`).
    - Clear separation between network I/O, parsing, and business rules.
    - Good single-responsibility boundaries overall.
  - **Cons / Risks**
    - A few ownership and lifecycle responsibilities are “spread” across classes (e.g. client destruction via `ErrorStrategy` while `MsgStrategy` still holds references).
    - Some types (`String`, `CmdStruct`, `ChannelKey`, constants like `MESSAGE`, `NEWCONN`, etc.) are defined in headers not yet reviewed here, but their usage seems consistent.

**Verdict**: The architecture is well thought out for a 42-style project and demonstrates solid understanding of event-driven server design. Most improvements now are in robustness and polish, not in major structural rewrites.

---

## 2. Core Server Loop & Event Handling

### 2.1 `IRCServer`

- `IRCServer` encapsulates:
  - Port/password + creation time.
  - A listening socket (`_serverFd`).
  - `EventDispatcher`, `ChannelManager`, `ClientManager`.
- `start()` → `setup()` + `run()`:
  - `setup()` creates the socket, sets `SO_REUSEADDR`, binds & listens, then calls `_eventDispatcher.init(_serverFd)`.
  - `run()` loops on `while (g_running)` and delegates to `_eventDispatcher.handleEvents()`.
  - `stop()` orchestrates server/client disconnection via `EventDispatcher`.
- `main.cpp` installs signal handlers (`SIGINT`, `SIGPIPE`) and uses `InputValidator::validateArgs`.

**Strengths**
- Resource handling is mostly correct:
  - `stop()` is called in destructor with try/catch.
  - Server FD is registered once and cleaned up by `EventDispatcher`.
- Use of `g_running` + signal handler (`Utils::handleSignal`) is simple and effective.

**Potential Improvements**
- **Destructor / stop idempotency**:
  - Destructor already calls `stop()` in a `try` block; `main()` also calls `server.stop()` after `start()`. Ensure `stop()` is idempotent (currently, it relies on `disconnectServer()` handling repeated calls gracefully — which it does, but the implicit assumption might surprise readers).
  - Consider documenting that double-stop is safe.
- **Error propagation**:
  - In `IRCServer::setup`, any failure throws `IRCException::ServerError`. This is fine, but you rely entirely on `main()`’s `catch (std::exception&)`. It’s OK, but you might later want more granular exit codes or logs.

### 2.2 `EventDispatcher` & Strategies

- `EventDispatcher` holds:
  - `_serverFd`, `_server`, `PollManager _pollManager`, and a map of `int -> IEventStrategy*` (`MESSAGE`, `ERROR`, `NEWCONN`).
- `handleEvents()`:
  - Calls `_pollManager.wait()` and throws `ServerError` on failure.
  - Iterates through `pollfd`s and chooses a strategy based on `revents` and FD:
    - `NEWCONN` when `POLLIN` on `_serverFd`.
    - `MESSAGE` for other FDs with `POLLIN`.
    - `ERROR` for `POLLHUP | POLLERR`.
  - Delegates to the strategy’s `handleEvent(fd, _pollManager, _server)`.

**Strengths**
- Clear separation of concerns:
  - `AcceptStrategy` only handles new connections.
  - `MsgStrategy` focuses on buffered reads and command dispatch.
  - `ErrorStrategy` handles disconnect/cleanup.
- `disconnectAllClients()` iterates through pollfds and calls `disconnectClient` — good reuse of the strategy for cleanup.

**Important Subtlety**
- `PollManager::getPollFds()` currently returns a **non-const reference**, and `handleEvents()` iterates directly over it:
  - At the same time, `ErrorStrategy::handleEvent` calls `pollManager.removeFd(fd)`, which erases from `_fds` inside `PollManager`.
  - This can cause **iterator invalidation** inside `EventDispatcher::handleEvents()` (depending on how `poll()` and removal lines up).
  - In practice, you do not see immediate UB because the pattern may “work” most of the time, but it’s fragile.

**Recommendation**
- Make `PollManager` APIs safer:
  - Change `std::vector<struct pollfd> &getPollFds()` to return by value or return a `const std::vector<struct pollfd> &` and guarantee you won’t erase from it while iterating.
  - A robust pattern:
    - `std::vector<struct pollfd> fds = _pollManager.getPollFds();`
    - Iterate over `fds`, but use the live `_pollManager` only for operations (disconnect/remove).
  - This is conceptually already described in `reviiew.md` for earlier fixes; ensure the implementation matches that description.

---

## 3. Client & Channel Management

### 3.1 `Client` and `ClientManager`

- `Client` stores:
  - FD, socket address, username/nickname/realname/hostname.
  - Joined channels (`std::set<Channel *>`), auth and registration flags.
  - Helper methods: prefix creation, checking registration state, welcome messages, and reply sending.
- `ClientManager`:
  - Maps by `fd` and by `nickname`.
  - Provides `addClient`, `removeClient`, `getClientByFd`, `getClientByNick`, `changeNick`, and a `createClient` factory.

**Strengths**
- Clear, simple life cycle:
  - `AcceptStrategy` calls `addClient()` + `createClient()`.
  - `ErrorStrategy` calls `removeClient(fd)` and is responsible for `delete client`.
- `changeNick` carefully maintains `_clientsByNick`, with uniqueness checks.
- `sendWelcome`, `broadcastJoinedChannels`, `getPrefix` encapsulate protocol-specific logic in `Client`, which is good.

**Concerns / Suggestions**
- **Use-after-free risks when commands disconnect clients**:
  - Many commands (e.g. invalid PASS, QUIT, etc.) can cause disconnect through `EventDispatcher::disconnectClient`, which eventually calls `ClientManager::removeClient(fd)` and deletes the `Client`.
  - `MsgStrategy::processMsg` holds a `Client *client` while iterating lines and sending to `CmdDispatcher`. After a command that disconnects, the pointer is stale.
  - You partly handle this by re-checking `clientManager.getClientByFd(fd)` after dispatch and early returning when it’s gone (and erasing `_sockBuffer`).
  - **Recommendation**:
    - Re-fetch the client pointer inside the loop before **each** dispatch; if it is already gone, just stop processing further lines.
    - This is a minor rework but makes the logic obviously safe.
  - Also consider centralizing “who is allowed to delete `Client` objects” and documenting it (currently: `ErrorStrategy` and possibly other flows that call it indirectly).

- **Error logging**:
  - `Client::sendReply` doesn’t check `send()`’s return value; for an educational project this is OK, but be aware it can silently fail in real-world scenarios.

### 3.2 `Channel` and `ChannelManager`

- `Channel` manages:
  - Name, password, topic, members, operators, invited users, modes, and user limit.
  - Broadcast, operator checks, user limit logic, channel mode representation, etc.
- `ChannelManager`:
  - Owns channels by name (heap allocated, freed in destructor and `deleteChannel`).
  - Handles `joinChannel`, `partChannel` (with/without message), `partAll`, replies for JOIN, and channel name/key validation.

**Strengths**
- `Channel` encapsulation is strong:
  - Simple and clear API for operations (`addUser`, `deleteUser`, `addOperator`, `broadcastToChannel`, etc.).
  - Correct semantics for default modes: channels created with `+t`, and `+k` if password is set.
  - `getUsersList` includes `@` for operators.
- `ChannelManager::joinChannel`:
  - Nicely checks for invite-only, key mismatch, and user limit.
  - Uses semantic errors (`ERR_INVITEONLYCHAN`, `ERR_CHANNELISFULL`, etc.).

**Minor Suggestions**
- `Channel::getChannelUsers` returns a reference to the internal set, which is then copied by some call sites (e.g. `broadcastToChannel` makes a local copy). This is OK but slightly inconsistent — either always operate on the original set (with care for modification) or always copy when iterating.
- `ChannelManager::partAll` uses `client->getJoinedChannels()` (a set) and iterates while calling `partChannel`, which again touches those sets. You copy the channel set at the start (good), so you’re safe from iterator invalidation.

---

## 4. Command Parsing and Dispatch

### 4.1 `CmdParser`

- Validates:
  - Raw line length (max 510 characters).
  - Optional prefix (`:nick!user@host`) by skipping it.
  - Command name: must be alphabetic, then uppercased.
  - Parameters and trailing text following RFC-style constraints (via `isValidMiddle` and `isValidTrailing`).
- Uses `std::istringstream` to parse tokens, stops at first “trailing” or after 14 parameters.

**Strengths**
- Defines clear syntactic constraints for middle and trailing parameters.
- Enforces IRC length limits and character restrictions.
- Converts command names to upper case early, simplifying downstream logic.

**Potential Improvements**
- `parseParams` has slightly complex logic around `TOPIC` and `:` handling:
  - `if (token[0] == ':' && cmdName != "TOPIC") tmp_trailing = token.substr(1); else tmp_trailing = token;`
  - There may be edge cases where a TOPIC that starts with `:` is treated differently than other commands. If that’s intentional (for 42 subject), document it; otherwise consider unifying the behavior.
- The parameter limit (14) is hard-coded; if you rely on that for subject compliance, it may be worth a short comment referencing the RFC or 42 requirement.

### 4.2 `CmdRegistry` & `CmdDispatcher`

- `CmdRegistry`:
  - Registers each command by name (`"PASS"`, `"NICK"`, `"JOIN"`, etc.) with `new` concrete command objects.
  - Destructor deletes them and clears the map.
- `CmdDispatcher::dispatch`:
  - Parses a raw line into `CmdStruct`.
  - Fetches handler from `_registry`.
  - Executes `handler->execute(client, command, server)`, or replies with `ERR_UNKNOWNCOMMAND`.

**Strengths**
- Very simple and extensible: to add a new command, you add includes and register in `CmdRegistry`’s constructor.
- Logging for invalid and unrecognized commands is a nice touch.

**Minor Suggestions**
- `CmdRegistry::_commands` is `std::map<String, ACommand*>` built at startup. For such a small command set this is fine, but if you wanted to be more modern C++, `std::map<String, std::unique_ptr<ACommand>>` would express ownership better. Given the project constraints (`-std=c++98`) raw pointers are expected, so this is fine.

---

## 5. Strategies & Message Handling

### 5.1 `MsgStrategy`

- Maintains `_sockBuffer` per FD.
- `handleEvent`:
  - Reads from socket into buffer.
  - Checks total buffer length (disconnect if > 2048 bytes).
  - Processes completed lines terminated by `\r\n`, trims them, enforces 510-char per-line limit, and dispatches commands.
- After each dispatched command, re-checks whether the client still exists before processing further data for that FD.

**Strengths**
- Sensible buffering logic:
  - Handles partial and multiple messages in a single `recv`.
  - Protects against input flooding by length checks.
- Clear separation between:
  - `readFromSock`, `checkBuffLength`, `processMsg`, and `disconnect`.

**Important Notes / Suggestions**
- The re-check of `clientManager.getClientByFd(fd)` after each dispatch is very good and essential to avoid use-after-free.
- Consider the additional refinement mentioned earlier:
  - Re-fetch `client` inside the `while` loop or at least set `client = clientManager.getClientByFd(fd)` before each dispatch to always operate on a fresh pointer.
- `disconnect` logs an error whenever `bytes != 0`, including cases where you call it manually with `bytes = -1` for “too much data”. That’s OK, but if you want clearer logs, you might differentiate “controlled disconnect” vs real `recv()` failure.

### 5.2 `AcceptStrategy`

- Accepts a new client:
  - Uses `accept(fd, ...)`.
  - On failure, throws `ServerError`.
  - On success, adds to `PollManager` and `ClientManager`.

**Suggestion**
- Consider setting `clientFd` to non-blocking (`fcntl` + `O_NONBLOCK`) in a more advanced server. For the project, using blocking sockets with `poll` is accepted, but performance under load is limited.

### 5.3 `ErrorStrategy`

- Handles disconnect of a single client:
  - Calls `ChannelManager::partAll(client)` to remove it from all channels.
  - Removes the FD from `PollManager`.
  - `ClientManager::removeClient(fd)` (deletion).
  - Closes the FD and logs.

**Strengths**
- Centralized disconnect logic is clear and consistent.

**Suggestions**
- Ensure there is exactly **one** place that ever deletes a `Client` object (currently, `removeClient` inside `ErrorStrategy`’s flow). That assumption is important across the codebase.
- Be cautious when calling `disconnectClient(fd)` from commands (like `PASS`) while they still hold `Client*` references. The existing re-check in `MsgStrategy` helps but doesn’t protect code inside the command itself if it keeps using the same pointer after `disconnectClient` returns.
  - In `PASS`, you call `disconnectClient` and then immediately `return`, which is safe. Keep that pattern strict in similar situations.

---

## 6. Commands: Behavior & Validation

### 6.1 `PASS`

- Validates:
  - Client pointer non-null.
  - Already authenticated → `ERR_ALREADYREGISTRED`.
  - Missing parameters → `ERR_NEEDMOREPARAMS`.
  - Wrong password → `ERR_PASSWDMISMATCH` then `disconnectClient`.
- On success, sets `client->setAuth(true)` and logs.

**Strengths**
- Properly enforces password requirement and prevents re-PASS after authentication.
- Clean early returns, no fall-through.

**Note**
- Using `ERR_PASSWDMISMATCH` for “PASS required” (in NICK) is a bit semantically odd, but within project specs it might be acceptable.

### 6.2 `NICK`

- Enforces:
  - Param presence (`ERR_NONICKNAMEGIVEN`).
  - Auth required (`ERR_PASSWDMISMATCH(..., "required")`).
  - Valid nickname characters and length.
  - Uniqueness, using `ClientManager::getClientByNick` and a reserved `"bot"` nick.
  - Uses lowercase `newNick`; validation respects IRC-style character sets.
- Registration logic:
  - If first login (`hasNick` + `hasUser` but not registered) → mark registered and send welcome replies.
  - If already registered and nick changes → send `RPL_SUCCNICK` and broadcast to all joined channels.

**Strengths**
- Good correctness for typical IRC semantics.
- Handles first registration vs nick change well.

**Suggestions**
- `isNickAvalible` is spelled with a typo; consider renaming to `isNickAvailable` for clarity.
- Logging of nick changes could be helpful for debugging (optional).

### 6.3 `JOIN`

- Validates:
  - Registration (`ERR_NOTREGISTERED`).
  - Param presence (`ERR_NEEDMOREPARAMS`).
  - “0” special case to part all channels.
  - Channel names and keys via `ChannelManager::isValidChannelName` and `isValidKey`.
- Uses `parseChannels` to build `(channel, key)` pairs and then calls `channelManager.joinChannel`.

**Strengths**
- Clean separation of parsing channels/keys and joining logic.
- Good error reporting when channel names/keys are invalid.

### 6.4 `PRIVMSG`

- Validates:
  - At least one param (target) → `ERR_NORECIPIENT`.
  - Non-empty trailing message → `ERR_NOTEXTTOSEND`.
- DCC SEND (file transfer) detection via `"\x01DCC SEND"` in trailing; forwards to recipient via `sendFile`.
- Special “bot” target:
  - Uses `botHandle` to provide canned responses for greetings, jokes, etc.
- Normal message flow:
  - Multiple comma-separated targets (up to 20); errors with `ERR_TOOMANYTARGETS` beyond that.
  - For channel targets:
    - Must exist; otherwise `ERR_NOSUCHNICK` (a bit odd; `ERR_NOSUCHCHANNEL` might be more semantically accurate).
    - Must be joined to send; otherwise `ERR_CANNOTSENDTOCHAN`.
  - For nick targets:
    - Resolves `Client` by nick; if missing, `ERR_NOSUCHNICK`.

**Strengths**
- Rich behavior for a key command (`PRIVMSG`), including DCC and a simple bot.
- Good input validation, plus numeric error replies.

**Suggestions**
- Consider using `ERR_NOSUCHCHANNEL` instead of `ERR_NOSUCHNICK` when the target is a channel name and doesn’t exist (to align better with RFC semantics).
- DCC handling is minimal (forwarding only), which is OK for this project — but it could be documented in `README.md` or comments.

---

## 7. Input Validation & Exceptions

### 7.1 `InputValidator`

- `validateArgs`:
  - Requires exactly 3 args (program, port, password), otherwise throws `InputError`.
  - `validatePort`:
    - Ensures numeric-only, uses `strtol`, checks ranges and `errno`, plus explicit 1024–65535 range via `PORT_MIN_RANGE`/`PORT_MAX_RANGE`.
  - `validatePassword`:
    - Enforces minimum length of 8 characters.

**Strengths**
- Thorough input checks with clear error messages.
- Good use of exceptions for early rejection.

### 7.2 `IRCException` hierarchy

- `IRCException::IRCException` base with `what()` override.
- Derived `InputError` and `ServerError` prefix their message with `"Input Error: "` and `"Server Error: "`.
- Global `catch (std::exception &e)` in `main` prints error and returns `EXIT_FAILURE`.

**Notes**
- The implementation in `IRCException.cpp` uses `IRCException::IRCException::IRCException` etc., which is a bit verbose/confusing but valid; consider simplifying with `namespace IRCException { ... }` and a more concise definition style.

---

## 8. Utilities, Types, and Misc

- `Utils`:
  - String trimming (`ltrim`, `rtrim`, `trim`), case-conversion (`strToUpper`, `strToLower`), splitting by delimiter, socket/poll struct helpers, and signal handling.
  - Global `g_running` for server loop control.
- `PollManager`:
  - Thin wrapper over `poll()`, storing FDs and events.

**Strengths**
- `Utils` centralizes small helpers; keeps other modules cleaner.
- `toString` template via `ostringstream` is idiomatic.

**Recommendations**
- Tighten `PollManager` encapsulation as discussed earlier (return copies or const references to `_fds`).
- Consider adding simple safety checks in `wait()` for empty `_fds` (although `poll` on zero fds is allowed, but may not be what you want).

---

## 9. Code Style & Maintainability

**Positives**
- Naming is generally consistent and descriptive (`ClientManager`, `ChannelManager`, `MsgStrategy`, etc.).
- Headers are well-guarded, and includes are organized through `IRC.hpp` re-export.
- There is a consistent pattern of early returns in command handlers, which keeps them readable.
- The `README.md` is clear and accurately describes the high-level architecture and patterns.

**Minor Issues / Suggestions**
- **Typos**
  - File name `AccpetStrategy.cpp` is misspelled; consider renaming to `AcceptStrategy.cpp` for clarity.
  - `isNickAvalible` → `isNickAvailable`.
- **Inheritance declaration**
  - `MsgStrategy`, `ErrorStrategy`, and `AcceptStrategy` use `class X : implements IEventStrategy`. In standard C++ this should be `class X : public IEventStrategy`. If `implements` is a macro, it’s a bit surprising; if not, this may be relying on a non-standard extension or is just accepted by your compiler in a permissive mode. For portability, prefer `public`.
- **Const-correctness**
  - Methods that do not modify object state (`getClientByFd`, `getClientByNick`, many getters) could be marked `const` where appropriate at the class level; this would improve API clarity.
- **Logging consistency**
  - You already have good log messages (`[INFO]`, `[WARNING]`, `[ERROR]`). Consider standardizing the format and maybe centralizing logging to a helper function or macro for easier changes later.

---

## 10. Summary of Key Recommendations

1. **PollManager safety**
   - Return a **copy** or a **const reference** from `getPollFds()` and avoid mutating `_fds` while iterating inside `EventDispatcher::handleEvents()`. This prevents potential iterator invalidation.

2. **Client lifetime & message processing**
   - In `MsgStrategy::processMsg`, re-fetch `Client *client` from `ClientManager` for each loop iteration before dispatching, and stop processing immediately if it’s null. This makes the use-after-free protection explicit and robust.
   - Maintain the convention that all `Client` deletions go through `ErrorStrategy` / `ClientManager::removeClient`.

3. **Command semantics & replies**
   - Consider using `ERR_NOSUCHCHANNEL` instead of `ERR_NOSUCHNICK` when a channel target doesn’t exist in `PRIVMSG`.
   - Optionally clarify TOPIC’s special handling in `CmdParser::parseParams` with a short comment.

4. **Portability & style cleanups**
   - Replace `implements` with `public` in strategy class declarations, unless you have a deliberate macro for it.
   - Fix small typos (`AccpetStrategy.cpp`, `isNickAvalible`) and add a bit more const-correctness where trivial.

5. **Documentation**
   - The existing `README.md` is already strong. Consider adding a short section on:
     - How disconnections flow through `ErrorStrategy`.
     - How `MsgStrategy` protects against use-after-free when commands disconnect clients.
     - Special features like the `bot` and DCC forwarding in `PRIVMSG`.

---

## 11. Overall Assessment

The `ft_irc` codebase is **well-structured, idiomatic C++98**, and demonstrates a solid understanding of event-driven network programming, IRC semantics, and object-oriented design patterns. The main architecture is sound; the remaining work is largely about **hardening edge cases, clarifying ownership and lifetime, and polishing style and documentation**. With the small adjustments outlined above, this project should be robust, maintainable, and easy to extend with new IRC features. 


