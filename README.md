# Semester 6 — Computer Networks & Workshop Coursework

Two parts: **CN** (Computer Networks lab assignments, in C) and
**Workshop** (short algorithm/OOP exercises in C and Java).

## Repo structure

```
Sem-6/
├── CN/
│   └── Lab/
│       ├── lab codes/              foundational socket programs
│       │   ├── echo_tcp_client.c / echo_tcp_server.c
│       │   ├── echo_udp_client.c / echo_udp_server.c
│       │   ├── time_tcp_client.c / time_tcp_server.c
│       │   ├── weather_udp_client.c / weather_udp_server.c
│       │   ├── error_client.c / error_server.c
│       │   ├── FTP_single_client.c / FTP_single_server.c
│       │   ├── FTP_multi_client.c / FTP_multi_server.c
│       │   ├── R_DVR.c                 Distance Vector Routing
│       │   └── R_LSR.c                 Link State Routing (Dijkstra)
│       ├── Lab-6/                      TCP client/server (compiled binaries included)
│       ├── Lab-7/                      multi-client file-transfer server + sample transferred files
│       ├── Lab_8/
│       │   ├── dsr.c                   Dijkstra-based link-state routing
│       │   └── lsr.c                   distance-vector routing
│       └── SE22UCSE078_CN_A2.pdf … A5.pdf   submitted assignment reports
└── Workshop/
    └── Sem-6/
        ├── Day-2/
        │   ├── Bitonic.c                max subarray / bitonic-style array problem
        │   └── Jump.c                   greedy "farthest jump with ladders" problem
        └── Day-3/
            ├── Bank.java                 priority-queue-based interest allocation
            ├── BankAccount.java          abstract base class for accounts
            ├── SavingsAccount.java       concrete subclass
            └── BankingSystem.java        driver / demo program
```

## CN — Computer Networks labs

Progresses from basic sockets to full protocol simulations:

- **Echo / time / weather / error clients & servers** — foundational
  TCP and UDP socket programming (connection setup, request/response,
  basic error handling).
- **FTP single- and multi-client/server** — a simple file-transfer
  protocol, first single-client, then extended to handle multiple
  clients concurrently.
- **Lab-6 / Lab-7** — further TCP client/server exercises; Lab-7 is a
  multi-client file-transfer server (sample transferred files included
  as `Client*-File-1`).
- **Routing protocol simulations** (`R_DVR.c`, `R_LSR.c`, and `Lab_8/`) —
  console-driven simulations of **Distance Vector Routing** and
  **Link-State Routing** (Dijkstra's algorithm) over a user-entered cost
  matrix, printing the resulting routing tables.

Submitted lab reports (`SE22UCSE078_CN_A2–A5.pdf`) document each
assignment's setup and output.

### Compiling

```bash
gcc -o echo_tcp_server echo_tcp_server.c
gcc -o echo_tcp_client echo_tcp_client.c
./echo_tcp_server &
./echo_tcp_client
```
Server/client pairs generally need to be run together (server first, in
the background or a separate terminal), on the same host with matching
port numbers (see the `#define PORT` at the top of each file).

## Workshop — algorithm & OOP exercises

- **Day-2 (C):** `Bitonic.c` and `Jump.c` are short standalone algorithm
  exercises (array/greedy-style problems) — compile and run directly:
```bash
  gcc -o jump Jump.c && ./jump
```
- **Day-3 (Java):** a small banking-system OOP exercise — an abstract
  `BankAccount` base class, a `SavingsAccount` subclass, and a
  `BankingSystem` driver, plus a separate `Bank.java` demonstrating
  priority-queue-based interest allocation across accounts:
```bash
  javac BankAccount.java SavingsAccount.java BankingSystem.java
  java BankingSystem
```

## Notes

- `.DS_Store`, `.vscode/settings.json`, and the compiled binaries in
  `Lab-6/`, `Lab-7/` (`client`, `server` with no extension) and `Day-3/`
  (`.class` files) are build artifacts from the original machine — fine to
  `.gitignore` and recompile from source rather than committing.
