/* exp_util.h — запуск потенційно "падаючого" коду в дочірньому процесі.
 * Батько перехоплює stderr дитини і друкує тільки рядки-діагнози (ASan/UBSan SUMMARY),
 * тож експеримент показує реальну реакцію санітайзера, а не "повірте на слово". */
#ifndef EXP_UTIL_H
#define EXP_UTIL_H

#ifdef __APPLE__
#define _DARWIN_C_SOURCE
#else
#define _POSIX_C_SOURCE 200809L /* fdopen, strsignal під -std=c11 на glibc */
#endif
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static int run_in_child(const char *label, void (*fn)(void)) {
    int fds[2];
    if (pipe(fds) != 0) { perror("pipe"); return -1; }
    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return -1; }
    if (pid == 0) {
        close(fds[0]);
        dup2(fds[1], STDERR_FILENO);
        fn();
        fflush(stdout);
        _exit(0);
    }
    close(fds[1]);
    char buf[4096];
    FILE *in = fdopen(fds[0], "r");
    int shown = 0;
    while (fgets(buf, sizeof buf, in)) {
        if (strstr(buf, "ERROR: AddressSanitizer") || strstr(buf, "SUMMARY:") ||
            strstr(buf, "runtime error") || strstr(buf, "WRITE of size") || strstr(buf, "READ of size") ||
            strstr(buf, "is located")) {
            if (shown++ < 6) printf("      | %s", buf);
        }
    }
    fclose(in);
    int st;
    waitpid(pid, &st, 0);
    if (WIFEXITED(st))
        printf("   [%s] дочірній процес завершився з кодом %d\n", label, WEXITSTATUS(st));
    else if (WIFSIGNALED(st))
        printf("   [%s] дочірній процес вбито сигналом %d (%s)\n", label, WTERMSIG(st), strsignal(WTERMSIG(st)));
    return st;
}

#endif
