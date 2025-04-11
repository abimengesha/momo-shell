# xv6-shell-extension: Custom Shell (momo.c) in xv6

This project is a modified version of the xv6 operating system. It includes a custom-built Unix-style shell named `momo.c`, written in C and integrated into the `user/` space of the xv6 environment.

## 🐚 Project: `momo.c` – A Minimalist Shell

Located in: `user/momo.c`

`momo.c` is a lightweight command-line shell that supports core Unix-like functionality inside the xv6 OS. It was created to deepen understanding of system calls, process control, and user-space execution in a constrained OS environment.

### ✅ Features:
- Command execution with argument parsing
- Input/output redirection (`<`, `>`, `>>`)
- Piping (`|`) support
- Background execution using `&`
- Custom prompt with simple user feedback
- Integration with `fork`, `exec`, `wait`, `dup`, and file descriptor manipulation


🪪 Credits
Built on MIT’s xv6 operating system (based on Unix V6), modified by Abigail Mengesha for academic and portfolio purposes.
##FOGOS

📜 License
Distributed under the MIT License. See xv6-LICENSE for details.
