#!/usr/bin/python3

# Basically, the Python program compiles the C code, loads it into the kernel,
# and attaches it to the execve syscall kprobe. Whenever any application on this
# (virtual) machine calls execve() it triggers the eBPF hello() program, which
# writes a line of trace into a specific pseudofile. The Python program reads
# the trace message from the pseudofile and displays it to the user.

# BCC installation instructions:
# https://github.com/iovisor/bcc/blob/master/INSTALL.md

from bcc import BPF

f = open("hello.c", "r")
program = f.read()

b = BPF(text=program)
# execve is the syscall used to execute a program
syscall = b.get_syscall_fnname("execve")
b.attach_kprobe(event=syscall, fn_name="hello")

b.trace_print()

# hello.py -> user space part of the application
# hello()  -> eBPF program that runs in the kernel
