#include <stdio.h>

#define UTMP_FILE
#define WTMP_FILE

#include <sys/types.h>

#define ut_hname ut_user

struct utmp {
    char ut_user[32];
    char ut_id[14];

    char ut_line[32];
    short ut_type;
    pid_t ut_pid;

    struct exit_status {
        short e_termination;
        short e_exit;
    } ut_exit;

    time_t ut_time;
    char ut_host[64];
};