#!/usr/bin/env python3

from bcc import BPF
import sys
import psutil
import ctypes
import time
import signal

if len(sys.argv) != 2:
    print("Usage: {} <your_name>".format(sys.argv[0]))
    sys.exit(1)

name = sys.argv[1]

# Getting PIDs of processes whose command line arguments contain a specific name
pids = set()
for proc in psutil.process_iter(attrs=['cmdline']):
    try:
        cmdline = proc.info['cmdline']
        # Excluding the current script's process and monitor only processes that start with "./my_app" or a specific binary
        if cmdline and name in cmdline and cmdline[0].startswith("./my_app"):
            pids.add(proc.pid)
            # Logging detected processes for debugging
            print(f"Detected PID {proc.pid} with command line: {' '.join(cmdline)}")
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
        pass

if not pids:
    print("No processes found containing '{}' in command line arguments.".format(name))
    sys.exit(1)

# Printing the detected PIDs
print("\nMonitoring system calls for processes with PIDs: {}\n".format(', '.join(map(str, pids))))

# BPF program
bpf_text = """
#include <uapi/linux/ptrace.h>
#include <linux/sched.h>

struct sys_enter_args {
    __u64 unused;
    __u64 id;
    __u64 args[6];
};

struct data_t {
    u32 pid;
    u64 syscall_id;
};

BPF_HASH(target_pids, u32, u32);
BPF_PERF_OUTPUT(events);

int trace_syscalls(struct sys_enter_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    u32 *found = target_pids.lookup(&pid);
    if (found) {
        struct data_t data = {};
        data.pid = pid;
        data.syscall_id = ctx->id;
        events.perf_submit(ctx, &data, sizeof(data));
    }

    return 0;
}
"""

b = BPF(text=bpf_text)
b.attach_tracepoint(tp="raw_syscalls:sys_enter", fn_name="trace_syscalls")

# Initializing the hash map with target PIDs
for pid in pids:
    b["target_pids"][ctypes.c_uint(pid)] = ctypes.c_uint(1)

print("Tracing syscalls... Press Ctrl+C to exit.")

# Defining the data_t structure in Python
class Data(ctypes.Structure):
    _fields_ = [
        ("pid", ctypes.c_uint),
        ("syscall_id", ctypes.c_ulonglong),
    ]

# Getting syscall name mapping
from bcc.syscall import syscalls

def print_event(cpu, data, size):
    event = ctypes.cast(data, ctypes.POINTER(Data)).contents
    pid = event.pid
    syscall_id = event.syscall_id
    syscall_name = syscalls.get(syscall_id, "unknown")
    print(f"PID {pid} called syscall {syscall_id} ({syscall_name})")
    time.sleep(0.5)  # This small delay is to slow down the output for readability

# Setting up perf buffer
b["events"].open_perf_buffer(print_event)

# Handling KeyboardInterrupt gracefully
def signal_handler(sig, frame):
    print("\nDetaching and exiting gracefully...")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

# Output
try:
    while True:
        b.perf_buffer_poll(timeout=100)
except SystemExit:
        pass