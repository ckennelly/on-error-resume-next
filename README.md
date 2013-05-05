On Error Resume Next - More Elegant Error Handling for a More Civilized Age

Overview
========

This program is for **entertainment purposes only**.  It is distributed
**without any warranty**.  Upon receiving a fatal signal, the instruction
pointer for the program being run will be incremented by 1, i.e., resuming
next, and the error otherwise ignored.

When using On Error Resume Next, `SIGILL`, `SIGABRT`, `SIGBUS`, `SIGFPE`, and
`SIGSEGV` are ignored and the instruction pointer is incremented by one
whenever such a signal occurs.  Execution **behavior is undefined** when
continuing after ignoring one of these signals.  This can have **severe
consequences**, including but not limited to **data loss**, **data
corruption**, and
**[nasal demons](https://en.wikipedia.org/wiki/Nasal_demons)**.

Building
========

On Error Resume Next requires CMake 2.8 (or later) and a C compiler.

To acknowledge the lack of the warranty and to waive all claims arrising from
use of this program, you will need to define the preprocessor macro:
    `I_ACKNOWLEDGE_THIS_PROGRAM_IS_DISTRIBUTED_WITHOUT_WARRANTY_AND_WAIVE_ALL_CLAIMS_ARISING_FROM_USE_OF_THIS_PROGRAM`

Without this, execution will procede normally.  If you are not familiar with
defining preprocessor macros, you may want to refrain from using this program.

Usage
=====

To run a program with more elegant error handling, run:

    on-error-resume-next my-program arg1 arg2

A sample program (`sigsegv`) that raises the segmentation fault signal is
included under `samples/`:

    $ samples/sigsegv 
    Segmentation fault
    $ on-error-resume-next samples/sigsegv
    Caught signal (Segmentation fault), resuming next.
    Program exited with status 0.

Limitations
===========

On Error Resume Next uses ptrace to intercept signals destined for a child
process.  This prevents the use of other ptrace-based programs (`gdb`,
`strace`, etc.).
