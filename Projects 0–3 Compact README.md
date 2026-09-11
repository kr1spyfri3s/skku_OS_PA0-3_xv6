# SWE3004 Operating Systems — Projects 0–3

Implementation of **xv6-riscv** projects for **SWE3004 Operating Systems, Spring 2026** at Sungkyunkwan University.

The repository covers the first four projects of the course, progressing from basic xv6 booting to system calls, CPU scheduling, and virtual memory.

---

## Projects

| Project | Topic | Main Concepts |
|---|---|---|
| **Project 0** | Booting xv6 | xv6, RISC-V, QEMU, OS boot process |
| **Project 1** | System Calls | System-call interface, process management, memory |
| **Project 2** | CPU Scheduling | EEVDF, nice values, virtual runtime, deadlines |
| **Project 3** | Virtual Memory | `mmap()`, page faults, `munmap()`, demand paging |

---

## Project 0 — Booting xv6

Introduced the **xv6 RISC-V operating system** and its basic boot process.

### Implementation

- Built and executed xv6 using QEMU
- Modified the boot process to display:
  - Student ID
  - Name
  - Custom boot message

### Environment

```text
OS: xv6-riscv
Architecture: RISC-V
Emulator: QEMU
Development Environment: SKKU Ye Server
```

---

## Project 1 — System Calls

Extended xv6 by implementing new system calls for **process management, scheduling priority, and memory information**.

### Implemented System Calls

```c
int getnice(int pid);
int setnice(int pid, int value);
void ps(int pid);
uint64 meminfo(void);
int waitpid(int pid);
```

### Key Features

- Process nice-value management
- Process information display through `ps()`
- Available-memory measurement
- Waiting for a specific process
- Integration with xv6's user-to-kernel system-call path

### System Call Flow

```text
User Program
     ↓
user.h / usys.pl
     ↓
ecall
     ↓
syscall.c
     ↓
sysproc.c
     ↓
Process / Memory Management
```

---

## Project 2 — CPU Scheduling

Replaced xv6's default scheduling behavior with an implementation based on **EEVDF (Earliest Eligible Virtual Deadline First)**.

### Key Concepts

- Nice values: `0 ~ 39`
- Default nice value: `20`
- Scheduling weight
- Runtime
- Virtual runtime
- Virtual deadline
- Process eligibility
- Timer-based preemption

### Scheduling Flow

```text
Runnable Processes
       ↓
Check Eligibility
       ↓
Find Earliest Virtual Deadline
       ↓
Select Process
       ↓
Run for Time Slice
       ↓
Update Runtime / Vruntime
       ↓
Recalculate Deadline
```

The scheduler uses a **5-tick time slice** and maintains scheduling information for each process.

### `ps()` Enhancement

Process scheduling information was extended to include values such as:

```text
runtime
weight
vruntime
vdeadline
eligibility
total tick
```

---

## Project 3 — Virtual Memory

Extended xv6's virtual-memory subsystem by implementing **memory mapping and page-fault handling**.

### Implemented System Calls

```c
uint64 mmap(
    uint64 addr,
    int length,
    int prot,
    int flags,
    int fd,
    int offset
);

int munmap(uint64 addr);

int freemem(void);
```

### Supported Mapping Types

- Anonymous memory mapping
- File-backed memory mapping
- `MAP_POPULATE`
- Lazy allocation through page faults

### Memory Mapping Flow

```text
mmap()
  │
  ├── MAP_POPULATE
  │      ↓
  │   Allocate Pages
  │
  └── Without MAP_POPULATE
         ↓
      Register Mapping
         ↓
      Memory Access
         ↓
      Page Fault
         ↓
      Allocate Page
         ↓
      Map Physical Memory
```

### Page Fault Handling

The page-fault handler:

1. Identifies the faulting address
2. Determines the type of page fault
3. Locates the corresponding memory mapping
4. Checks access permissions
5. Allocates a physical page
6. Loads file data when required
7. Creates the corresponding page-table entry

### Process Support

Memory-mapped regions are also handled during `fork()`, allowing the child process to inherit the parent's mapped regions.

---

## Technologies

```text
C
RISC-V
xv6
QEMU
Git / GitHub
Linux
```

## Repository Structure

```text
xv6-riscv/
├── kernel/
├── user/
├── mkfs/
├── Makefile
└── README.md
```

## Learning Outcomes

Through Projects 0–3, I worked with progressively deeper layers of an operating system:

```text
Project 0
   ↓
OS Boot Process
   ↓
Project 1
   ↓
System Calls & Process Management
   ↓
Project 2
   ↓
CPU Scheduling
   ↓
Project 3
   ↓
Virtual Memory & Page Faults
```

These projects provided hands-on experience with **operating-system kernels, system calls, process scheduling, virtual memory, page tables, and RISC-V architecture**.

---

## Course

**SWE3004 Operating Systems**  
Sungkyunkwan University · Spring 2026

Based on the MIT **xv6-riscv** teaching operating system.