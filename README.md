# 🐚 minishell

> A functional UNIX shell written in C, built from scratch as part of the École 42 curriculum.

![Language](https://img.shields.io/badge/language-C-blue)
![School](https://img.shields.io/badge/school-42-black)
![Status](https://img.shields.io/badge/status-finished-brightgreen)

---

## 📖 Description

**minishell** is a simplified but functional UNIX shell, built entirely in C without using any existing shell code. It replicates core behaviors of **bash**, including command execution, pipes, redirections, environment variable expansion, and built-in commands.

This project is one of the most complex at École 42 — it requires a deep understanding of processes, file descriptors, signal handling, and parsing.

---

## ✨ Features

### 🔧 Command Execution
- Executes any binary found in `$PATH`
- Supports absolute and relative paths (`./program`, `/usr/bin/ls`)
- Handles command arguments

### 🔗 Pipes
- Full pipeline support: `cmd1 | cmd2 | cmd3 | ...`
- Each command runs in its own child process with connected stdin/stdout

### 🔀 Redirections
| Operator | Behavior |
|----------|----------|
| `>`  | Redirect stdout to file (overwrite) |
| `>>` | Redirect stdout to file (append) |
| `<`  | Redirect stdin from file |
| `<<` | Here-doc (read until delimiter) |

### 💬 Quoting
- `'single quotes'` — inhibit all interpretation
- `"double quotes"` — allow `$VAR` expansion

### 💲 Variable Expansion
- `$VAR` — expands environment variables
- `$?` — expands to the exit status of the last command

### 🔔 Signals
| Signal | Behavior |
|--------|----------|
| `Ctrl+C` | Interrupt current process, new prompt |
| `Ctrl+D` | Exit shell |
| `Ctrl+\` | Ignored (like bash) |

### 📦 Built-in Commands

| Command | Description |
|---------|-------------|
| `echo [-n]` | Print text, optional no newline |
| `cd [path]` | Change directory |
| `pwd` | Print working directory |
| `export [VAR=val]` | Set environment variable |
| `unset [VAR]` | Remove environment variable |
| `env` | Print environment variables |
| `exit [code]` | Exit the shell with optional exit code |

---

## 🗂️ Project Structure

```
minishell/
├── src/
│   ├── main.c              # Entry point, REPL loop
│   ├── parsing/            # Lexer, tokenizer, AST builder
│   ├── execution/          # Command execution, pipes, redirections
│   ├── builtins/           # Built-in command implementations
│   ├── signals/            # Signal handlers
│   └── utils/              # Helper functions
├── libft/                  # Custom standard library
├── include/minishell.h     # Main header
└── Makefile
```

---

## 🚀 Getting Started

### Requirements

- Linux or macOS
- `gcc`, `make`
- `libreadline-dev`

```bash
sudo apt-get install libreadline-dev
```

### Build & Run

```bash
# Clone the repo
git clone https://github.com/YOUR_USERNAME/minishell.git
cd minishell

# Build
make

# Run
./minishell
```

---

## 💡 Usage Examples

```bash
# Basic commands
minishell$ ls -la
minishell$ echo "Hello, World!"
minishell$ cat file.txt | grep "pattern" | wc -l

# Redirections
minishell$ echo "log entry" >> logfile.txt
minishell$ sort < unsorted.txt > sorted.txt

# Here-doc
minishell$ cat << EOF
> line 1
> line 2
> EOF

# Variable expansion
minishell$ export NAME="Matheo"
minishell$ echo "Hello $NAME, exit code: $?"

# Built-ins
minishell$ cd ~/projects
minishell$ pwd
minishell$ env | grep PATH
minishell$ exit 0
```

---

## 🏗️ Technical Highlights

- **Lexer/Parser** — tokenizes input into a structured command list, handles quotes, operators, and special characters
- **Abstract Syntax Tree (or command list)** — represents pipelines and redirections as a structured data format
- **Process management** — `fork()`, `execve()`, `waitpid()` for each command in a pipeline
- **File descriptor management** — careful `dup2()` usage to wire pipes and redirections, with proper cleanup
- **Signal handling** — `sigaction()` used to cleanly handle `SIGINT`, `SIGQUIT` in both interactive and child modes
- **Memory management** — full leak-free cleanup on exit (verified with Valgrind)

---

## 👥 Authors

- **Mathéo Vachon** — *École 42 Lyon* — [@mvachon](https://github.com/YOUR_USERNAME)

---

## 📚 Resources

- [The GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Writing your own shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
- `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`
