#!/usr/bin/python3

from bcc import BPF
from time import sleep

f = open("hello-buffer.c", "r")
program = f.read()

b = BPF(text=program)
syscall = b.get_syscall_fnname("execve")
b.attach_kprobe(event=syscall, fn_name="hello")

def print_event(cpu, data, size):
  data = b["output"].event(data)
  print(f"{data.pid} {data.uid} {data.command.decode()} " + \
        f"{data.message.decode()}")

b["output"].open_perf_buffer(print_event)
while True:
  b.perf_buffer_poll()
