#!/usr/bin/python3

from bcc import BPF
from time import sleep

f = open("../hello-map.c", "r")
program = f.read()

b = BPF(text=program)

syscall_1 = b.get_syscall_fnname("execve") # execute commands
syscall_2 = b.get_syscall_fnname("openat") # open files
syscall_3 = b.get_syscall_fnname("write") # write data to a file

b.attach_kprobe(event=syscall_1, fn_name="hello")
b.attach_kprobe(event=syscall_2, fn_name="hello")
b.attach_kprobe(event=syscall_3, fn_name="hello")

while True:
  sleep(2)
  s = ""
  for k,v in b["counter_table"].items():
    s += f"ID {k.value}: {v.value}\t"
  print(s)
