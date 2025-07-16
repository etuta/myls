# myls

A simplified reimplementation of the Unix `ls` command in C.

## Overview

`myls` lists files and directories with optional flags to:

- show hidden files (`-a`)
- display detailed file information (`-l`)

It demonstrates usage of:
- Directory traversal (`opendir`, `readdir`)
- File metadata (`stat`)
- String manipulation
- Permissions formatting

This project is for educational purposes and does **not** replicate every feature of GNU `ls`.

---

## Usage

```
./myls [options] [file|directory...]
```

### Options

- `-a`  
  Include hidden files (those starting with `.`).
  
- `-l`  
  Display detailed information (permissions, owner, group, size, timestamp).

You can combine flags (`-al` or `-la`).

### Examples

List files in the current directory:

```
./myls
```

List all files, including hidden ones:

```
./myls -a
```

List detailed info:

```
./myls -l
```

List detailed info including hidden files:

```
./myls -al
```

List a specific directory:

```
./myls /path/to/directory
```

List a specific file:

```
./myls -l /path/to/file
```

---

## How It Works

- **Option parsing**  
  Uses `getopt()` to handle `-a` and `-l`.
  
- **Directory processing**  
  - If no argument is given, opens the current directory.
  - If arguments are provided, iterates through each one:
    - If the argument is a directory, lists its contents.
    - If the argument is a file, displays information for that file.

- **Permissions formatting**  
  Converts `st_mode` into human-readable strings (e.g., `-rw-r--r--`).

- **Detailed output** (`-l`):
  - File type and permissions
  - Number of hard links
  - Owner and group
  - File size
  - Modification time
  - Filename

---

## Compilation

Use `gcc` to compile:

```
gcc -o myls myls.c
```

---

## Limitations and Notes

- Symbolic links are not specially handled.
- No support for recursive directory traversal.
- No color output.
- No sorting.
- Minimal error handling for unusual cases.

---

## Example Output

```
$ ./myls -l
-rw-r--r-- 1 user group    512 Jul 15 12:34 file1.txt
drwxr-xr-x 2 user group   4096 Jul 15 12:35 folder
```

---

## Author

Eliza Tuta
