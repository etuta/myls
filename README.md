# myls: Custom Directory Listing Tool

A reimplementation of the `ls` command in C for Linux systems.  
This project provides a deeper understanding of file system structures and access permissions, which are fundamental concepts in **system administration and IT operations**.

---

## Overview

This tool demonstrates:

- Programmatic access to filesystem metadata.
- Detailed inspection of file permissions, ownership, and timestamps.
- Handling hidden files and directory entries.
- Parsing command-line options (`-a`, `-l`), similar to GNU coreutils.

By building and using `myls`, IT professionals and students gain insight into:

- How file metadata is retrieved (`stat`).
- Permission bits and security considerations.
- Directory traversal and filtering.

---

## Why It Matters for IT

System administrators routinely:

- Audit permissions and ownership.
- Generate reports on file metadata.
- Troubleshoot access issues.

This project shows **how such utilities work internally**, reinforcing critical skills for:

- Securing Linux servers.
- Automating file and directory inspections.
- Developing custom monitoring tools.

---

## Building

Use `make` to compile:

```
make
```

This creates:

- `myls`: executable binary.

Clean up:

```
make clean
```

---

## Usage

List files in a directory:

```
./myls [options] [directory]
```

Options:

- `-a`: show all entries, including hidden files.
- `-l`: long listing format with permissions, ownership, and timestamps.

Example:

```
./myls -al /var/log
```

---

## Example Output

```
drwxr-xr-x 2 root root     4096 Jun 12 14:21 .
drwxr-xr-x 6 root root     4096 Jun 12 14:19 ..
-rw-r--r-- 1 root root    12345 Jun 12 14:20 syslog
```

---

## Technical Details

- Uses `opendir()` and `readdir()` for directory traversal.
- `stat()` retrieves metadata.
- Custom formatting replicates standard `ls -l` output.

---

## Educational Value

Building your own `ls` clone deepens understanding of:

- UNIX file permissions.
- Filesystem structures.
- Secure programming practices for system utilities.

This knowledge is applicable to:

- Writing **custom audit scripts**.
- Developing **automated reporting tools**.
- Enhancing **security posture** by understanding metadata manipulation.

---

## Authors

- Eliza-Teodora Tuta
- Tenzin Dolma Gyalpo
