# Email Communication Network Analysis in C

Engineered a C application to model and analyze email exchanges, constructing a directed weighted graph from raw email data to uncover communication patterns and statistics.

This project was built entirely in **C** using foundational data structures and manual memory management (`malloc`/`free`), without object-oriented features or standard library containers.

![Language](https://img.shields.io/badge/Language-C-A8B9CC.svg?logo=c&logoColor=white)
![Data Structure](https://img.shields.io/badge/Data_Structure-Graph_(Adjacency_List)-00758F.svg)
![Algorithm](https://img.shields.io/badge/Algorithm-DFS_|_BFS-orange.svg)

---

## 🏛️ System Architecture

The core of the application is a custom-built, directed, weighted graph. This graph is implemented in C as an **adjacency list**, which is highly efficient for sparse graphs (like social networks) and managed entirely with pointers and dynamic memory.

* **Vertices (Nodes):** Each unique user (sender or recipient) is represented as a vertex in the graph. Each vertex node stores metadata, such as the user's name, total number of emails sent, and the aggregate word count of all emails they have received.
* **Edges (Connections):** A directed edge from `User A` to `User B` is created if A sends an email to B.
* **Edge Weights:** The weight of an edge represents the **cumulative word count** of all emails sent from one user to another. The system is designed to update this weight, summing the word counts if `User A` sends multiple emails to `User B`.

> **[Image: A simple directed graph with 3 nodes and weighted edges]**
>
> *(**Developer Note:** A small diagram showing A -> B (w: 150) and B -> C (w: 200) would be perfect here to illustrate the concept.)*

---

## ✨ Key Functionalities

The application is modular, with C functions built to handle each part of the pipeline:

1.  **`readEmails()`**
  	 * Parses a directory of `.txt` email files.
  	 * For each email, it identifies the sender and recipient(s).
  	 * It counts the number of words in the email body.
  	 * It calls `createVertex()` and `createEdge()` to build or update the graph.

2.  **`createVertex()` / `createEdge()`**
  	 * Manages the graph's dynamic memory.
  	 * `createVertex` adds a new user to the primary vertex list if they don't exist.
  	 * `createEdge` adds a new node to a vertex's adjacency list. If an edge to that recipient already exists, it simply increments the edge's weight (cumulative word count).

3.  **`printGraph()`**
  	 * Outputs the complete adjacency list to the console.
  	 * It iterates through each vertex (user) and then traverses their linked list of outgoing edges, printing the destination user and the total word count (weight) for that connection.

4.  **`checkPath()`**
  	 * Implements a graph traversal algorithm (Depth-First Search or Breadth-First Search).
  	 * Given two users, this function determines if a communication path (a chain of emails) exists from the first user to the second, returning 1 if a path is found and 0 otherwise.

5.  **Network Statistics (in `main`)**
  	 * After the graph is built, the `main` function iterates through the structure to find and report key insights, such as:
    	 * The user who has **sent the most emails**.
    	 * The user who has **received the highest total word count**.

---

## 🚀 How to Compile & Run

This is a standard C application and must be compiled.

### 1. Compile
Using a C compiler like `gcc`:

```bash
# Compile the source file and create an executable named "email_analyzer"
gcc email_analyzer.c -o email_analyzer
```

### 2. Run
Execute the program from your terminal:

```bash
./email_analyzer
```

### Example `printGraph` Output

When run, the `printGraph` function will produce an output similar to this, representing the full adjacency list:

```
COMMUNICATION NETWORK:
---------------------------
User: 'alice@example.com' (Sent: 5 | Received Words: 120)
  -> 'bob@example.com' (Words: 350)
  -> 'charlie@example.com' (Words: 200)

User: 'bob@example.com' (Sent: 3 | Received Words: 350)
  -> 'dave@example.com' (Words: 150)

User: 'charlie@example.com' (Sent: 2 | Received Words: 200)
  -> 'alice@example.com' (Words: 120)
  -> 'bob@example.com' (Words: 50)

User: 'dave@example.com' (Sent: 1 | Received Words: 150)
  (No outgoing emails)
---------------------------
```