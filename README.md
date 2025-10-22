# Minishell (beta version)

![C](https://img.shields.io/badge/language-C-blue.svg)

### Overview

Minishell is a **Bash-like command interpreter** created by [biertisch](https://github.com/biertisch) and [pdrlrnc](https://github.com/pdrlrnc).

A **shell** allows users to interact with the operating system by entering commands. Minishell implements essential Bash functionality, including **command parsing and execution**, **redirections**, **pipes**, **logical operators**, **parentheses**, and **environment** and **signal handling**.

Developed as part of the 42 curriculum, the project strengthens students' understanding of **process control** and **file manipulation** in C, while providing hands-on experience in **systems programming**.

>**Note**: Minishell is in **testing phase** - some features may malfunction in specific cases.

---
### Features

* **Readline Loop**: collects user input using `readline`.
* **History Navigation**: recall previous commands using `↑↓`.
* **Lexer**: tokenizes raw user input.
* **Parser**: builds an **Abstract Syntax Tree (AST)** to represent command structure.
* **Variable Expansion**:
	* environment variables (`$VAR`)
	* last exit status (`$?`)
	* wildcards (`*`) for the working directory (**bonus feature**)
* **Redirections**:
	* input (`<`)
	* output (`>`)
	* append (`>>`)
	* heredocs (`<<`)
* **Builtin Commands**:
	* `echo` with option `-n`
	* `cd [path]`
	* `pwd`
	* `export`
	* `unset`
	* `env`
	* `exit`
* **Pipes** (`|`)
* **Logical Operators** (`&&`, `||`) and **Parentheses** for priority (**bonus feature**)
* **Environment Handling**: manages variables, works with or without inherited environment.
* **Signal Handling**: handles `SIGINT`, `SIGQUIT`, `SIGTERM`, and `EOF` similar to Bash.
* **Error Handling**: reports syntax, system, and internal errors, ensuring clean exit on fatal issues.

---
### Installation

**Requirements**: `gcc`, `make`

```
git clone https://github.com/biertisch/minishell.git
cd minishell
make
```

---
### Usage
```
./minishell
[type your command]
```

Example:
```
ls -l | grep *file && echo "Found Makefile in $PWD"
```

Shortcuts:
* `ctrl-C` - display new prompt
* `ctrl-L` - clear screen
* `ctrl-D` - exit shell

---
### About 42

Founded in 2013, 42 is a global network of schools providing high-quality, scalable software engineering education. Its project-based, peer-to-peer curriculum emphasizes hands-on programming, algorithmic thinking, and systems-level problem solving.

---
### License & Contact

This repository is open for learning and reuse. Contributions, suggestions, and discussions are welcome — feel free to open an issue or reach out.