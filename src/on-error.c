/**
 * On Error Resume Next - More Elegant Error Handling for a More Civilized Age
 * (c) 2013 Chris Kennelly <chris@ckennelly.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/elf.h>
#include <linux/ptrace.h>

void resume_next(int pid) {
    struct pt_regs regs;
    memset(&regs, 0, sizeof(regs));

    struct iovec io = {&regs, sizeof(regs)};
    ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &io);

    #ifdef __x86_64__
    regs.rip++;
    #elif __i386__
    regs.eip++;
    #else
    #error Unknown architecture
    #endif

    #ifdef I_ACKNOWLEDGE_THIS_PROGRAM_IS_DISTRIBUTED_WITHOUT_WARRANTY_AND_WAIVE_ALL_CLAIMS_ARISING_FROM_USE_OF_THIS_PROGRAM
    ptrace(PTRACE_SETREGSET, pid, NT_PRSTATUS, &io);
    #endif
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No program specified.\n\n"
            "Usage: %s program [args...]\n", argv[0]);
        return 1;
    }

    /* Fork child process. */
    int pid = fork();
    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, 0);
        raise(SIGSTOP);

        (void) execvp(argv[1], argv + 1);

        /* If we're still here, something went wrong. */
        fprintf(stderr, "Unable to exec: %s\n", argv[1]);
        return 1;
    } else if (pid < 0) {
        fprintf(stderr, "Unable to fork.\n");
        return 1;
    }

    /* Wait for child to signal itself. */
    int status;
    wait(&status);
    ptrace(PTRACE_CONT, pid, 0, 0);

    /* Enter loop of waiting for signals, handling errors, repeating... */
    do {
        int sig;
        wait(&status);
        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
            printf("Program exited with status %d.\n", status);
            return status;
        } else if (WIFSTOPPED(status)) {
            sig = WSTOPSIG(status);
        } else if (WIFCONTINUED(status)) {
            continue;
        } else if (WIFSIGNALED(status)) {
            break;
        } else {
            sig = 0;
            fprintf(stderr, "Child process %d paused with unknown return value "
                "%d.\n", pid, status);
        }

        /* Process signal. */
        switch (sig) {
            case SIGTRAP:
                /* Drop. */
                sig = 0;
                break;
            case SIGILL:
            case SIGABRT:
            case SIGBUS:
            case SIGFPE:
            case SIGSEGV:
                /* Ignore fatal signals and "handle" the error. */
                printf("Caught signal (%s), resuming next.\n", strsignal(sig));
                resume_next(pid);
                #ifdef I_ACKNOWLEDGE_THIS_PROGRAM_IS_DISTRIBUTED_WITHOUT_WARRANTY_AND_WAIVE_ALL_CLAIMS_ARISING_FROM_USE_OF_THIS_PROGRAM
                sig = 0;
                #endif
                break;
            default:
                /* Pass signal through. */
                break;
        }

        ptrace(PTRACE_CONT, pid, 0, (void*) sig);
    } while (1);

    return 0;
}
