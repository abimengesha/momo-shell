# 🛠 xv6-shell-extension: Building a Custom Unix Shell in C (`momo.c`)

This project is a modified version of the xv6 operating system. It includes a custom-built Unix-style shell named `momo.c`, written in C and integrated into the `user/` space of the xv6 environment.

## 🐚 Project: `momo.c` – A Minimalist Shell

Located in: `user/momo.c` [View momo.c source code] (https://github.com/abimengesha/momo-shell/blob/main/user/momo.c)

`momo.c` is a lightweight command-line shell that supports core Unix-like functionality inside the xv6 OS. It was created to deepen understanding of system calls, process control, and user-space execution in a constrained OS environment.

### ✅ Features:
The `momo.c` shell supports the following core features within xv6:

- 🧠 **Basic Command Execution**  
  Runs executables using `fork()` and `exec()`, parsing arguments based on whitespace.

- 💬 **Custom Prompt Display**  
  Shows the exit status, command number, and current working directory for each prompt.

- 📝 **Basic Built-In Commands**  
  - `cd` to change directories  
  - `exit` to exit the shell  
  - `#` to ignore lines/comments  
  - `history` displays the last 100 entered commands  
  - `!!`, `!n`, and `!cmd` to recall history commands

- 📜 **Scripting Mode**  
  When given a file as an argument (e.g. `./momo test.sh`), the shell reads and executes commands from the file.

- ✅ **Minimal Error Handling**  
  Gracefully handles non-existent commands and directories.

> 🚧 **Planned / In Progress**:  
> Features like full piping support, file redirection (`>`, `<`, `>>`), background execution (`&`), executable script support (`#!`), and `execvp`-based path resolution are in development for future versions.

---

### 📈 Why This Project Matters
This project deepened my understanding of:
- How system calls like `fork()` and `exec()` work behind the scenes
- File descriptor duplication and process scheduling
- Minimalist OS development with limited resources
- Debugging C programs without modern tools in an xv6 environment

---

### 🧠 Credits
- Built on MIT’s xv6 operating system (based on Unix V6), modified by Abigail Mengesha for academic and portfolio purposes.
- FOGOS

### 📜 License
Distributed under the MIT License. See `xv6-LICENSE` for details.

