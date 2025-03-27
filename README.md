# 🧪 sysplayground – My Low-Level UNIX Lab 🧠

> A curated collection of real-world system programming experiments in C.  
> From pipes to process control, from `dup2` to `execve`, this is a personal sandbox to understand how UNIX works at the syscall level.

---

## 🌱 Why this exists

I built this playground to:

- Understand how low-level Linux system calls work
- Master process control, file descriptors, and memory management
- Break things on purpose, fix them, and learn deeply by doing
- Prepare for building my own shells, interpreters, or daemons

📚 This is not a tutorial repo. It’s **code-as-notes**, structured chaos, and real exploration.

---

## 🧠 Key Concepts Explored

| File / Folder                     | What It Covers                                                |
|----------------------------------|----------------------------------------------------------------|
| `01_pipe_playground.c`           | Unidirectional communication with `pipe()` 🧵                 |
| `02_dup2_playground.c`           | Redirecting output/input streams with `dup2()` 🔁             |
| `03_execve_madness.c`            | Replacing processes via `execve()` & env manipulation 💣      |
| `04_pipechain_ls_wc.c`           | Multi-command piping (e.g. `ls | wc -l`) 🪝                    |
| `07_pipe_bidirectional.c`        | Attempted bidirectional pipes (and why they break) 🔄         |
| `09_file_fd_leak_hunt.c`         | Manual FD leak detection under edge conditions 🔍             |
| `10_my_own_execve_wrapper/`      | Test harness + `execve()` wrapper lib + binary runner 🧪      |
| `10_pipex_dup2_puzzle/`          | Complex `dup2()` redirections for chained commands 🧩         |
| `11_trace_child_exit_states/`    | Reading `wait()` + `WEXITSTATUS()` + child lifecycle 🔍       |
| `12_fd_cleanup_fix.c`            | Manual cleanup of opened FDs to prevent leaks 🧼              |
| `12_fd_cleanup_sim.c`            | Simulating what happens when you don’t clean up 🐛            |
| `13_simulate_grep_cat_wc.c`      | Rebuilding pipelines manually (`cat | grep | wc`) 🏗️         |
| `14_pipex_execve_test.c`         | Manual mini-shell reproduction with `pipe` + `execve()` 🧵     |

---

## 🧰 How to Run

Compile any file with:

```bash
gcc -Wall -Wextra -Werror filename.c -o filename
./filename
