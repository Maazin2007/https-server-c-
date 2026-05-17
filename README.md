# C++ Multithreaded HTTP Server

A minimal HTTP server built from scratch in C++ using raw POSIX sockets — no frameworks, no libraries. Handles concurrent connections via multithreading, parses HTTP/1.1 requests by hand, and shuts down gracefully on SIGINT.

Built as a systems programming project to understand what actually happens under the hood when a browser makes a request.

---

## Demo

> Live on AWS EC2 — `http://18.223.122.138:8080`

---

## Features

- Raw TCP socket server using `socket()`, `bind()`, `listen()`, `accept()`
- Hand-parsed HTTP/1.1 GET requests — no regex, just string operations
- Static file serving from `/www` with correct `Content-Type` headers
- Proper HTTP status codes — `200 OK`, `404 Not Found`
- Multithreaded — each client gets its own `std::thread`, no blocking
- Graceful shutdown — catches `SIGINT`/`SIGTERM`, exits cleanly
- Request logging with method, path, and status code

---

## Build & Run

**Requirements:** g++, C++17, Linux or macOS

```bash
# clone
git clone https://github.com/<your-username>/http-server
cd http-server

# compile
g++ -o server src/main.cpp src/server.cpp src/request.cpp src/response.cpp -I include -pthread -std=c++17

# run on port 8080
./server
```

Then open `http://localhost:8080` in your browser.

---

## Project Structure

```
server/
│
├── src/
│   ├── main.cpp          # entry point, signal handling
│   ├── server.cpp        # socket setup, accept loop
│   ├── request.cpp       # HTTP parsing, client handler
│   └── response.cpp      # file serving, response builder
│
├── include/
│   ├── server.h
│   ├── request.h
│   └── response.h
│
├── www/
│   ├── index.html        # homepage with benchmark chart
│   └── about.html        # project breakdown
│
└── README.md
```

---

## How It Works

**1. Socket setup**

Creates a TCP socket and binds it to port 8080. `SO_REUSEADDR` prevents "address already in use" errors on restart.

**2. Accept loop**

Loops on `accept()` which blocks until a client connects. Each accepted connection is immediately handed off to a new `std::thread` and the loop goes straight back to `accept()`.

**3. HTTP parsing**

Reads raw bytes from the socket into a buffer, converts to `std::string`, extracts the first line, splits by spaces to get method and path.

```
GET /about HTTP/1.1  →  method = GET, path = /about
```

**4. File serving**

Maps path to `/www` directory. `/` serves `index.html`. Opens the file with `ifstream`, builds a valid HTTP response with headers and body. Returns `404 Not Found` if the file doesn't exist.

**5. Graceful shutdown**

`SIGINT`/`SIGTERM` triggers a signal handler that sets `atomic<bool> running = false` and closes `serverFd` to unblock `accept()`. The loop exits on the next iteration.

---

## Benchmark

10 concurrent requests with a 1 second simulated delay per request:

| Mode | Total time | 
|---|---|
| Single threaded | ~10.2s |
| Multithreaded | ~1.1s |

**9x faster** under concurrent load.

Test it yourself:
```bash
# send 10 concurrent requests
for i in {1..10}; do curl -s -o /dev/null http://localhost:8080/ & done
```

---

## What I Learned

- How TCP connections actually work at the syscall level — `socket()`, `bind()`, `listen()`, `accept()` are the same calls every web server uses
- HTTP is just text over a socket — parsing it manually made the protocol click in a way that using Express never did
- Why `atomic<bool>` is necessary when multiple threads share a variable — a regular `bool` is a data race waiting to happen
- Why `accept()` blocks and how closing the file descriptor is the right way to unblock it during shutdown
- The difference between a file descriptor for the server socket vs a file descriptor for a client connection

---

## What's Next

- Thread pool — bound concurrency instead of unlimited threads
- Keep-alive connections — reuse TCP connections across multiple requests  
- Async I/O with `epoll` — handle thousands of connections without spawning thousands of threads
- HTTPS via OpenSSL

---

## Author

Built by Maazin Osama Khattak — [LinkedIn](https://linkedin.com/in/maazinkhattak) · [GitHub](https://github.com/Maazin2007)