# Pipex

This project will allow you to explore a UNIX mechanism in detail, one that you are already familiar with, by implementing it in your program.

## Set up

to add the submodule libft do

```sh
cd libft && git submodule update --init --recusive && cd ..
```

## Tester

You can test the code with the `test.sh` file. It also run valgrind to not run valgrind put `USE_VALGRIND=1` to 0.

## The code

- **Why two forks?**
  - The program needs two child processes so each command runs separately: the
    first child reads from the input file and writes to the pipe; the second
    child reads from the pipe and writes to the output file. Using two forks
    recreates the shell pipeline `< cmd1 infile | cmd2 > outfile`.

- **Why `dup2`?**
  - `dup2` redirects standard file descriptors. We replace `stdin`/`stdout` in
    each child so `cmd1` reads from the input file and writes to the pipe, and
    `cmd2` reads from the pipe and writes to the output file. This makes the
    commands run unchanged (they still use fd 0/1).

- **How command paths are found**
  - If a command contains a `/` it is treated as a path and checked for
    executability. Otherwise the program reads the `PATH` environment variable,
    tries each directory by joining it with the command name, and returns the
    first accessible executable — matching how a shell resolves commands.

