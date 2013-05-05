#include <signal.h>

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    raise(SIGSEGV);
    return 0;
}
