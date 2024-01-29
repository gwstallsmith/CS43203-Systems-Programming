#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>

void dump_utmp_file(char *filename);
void dump_utmp_record(struct utmp *record);
char *get_utmp_type_name(int type);

int main(int argc, char **argv)
{
    if (argc == 1)
        dump_utmp_file(UTMP_FILE);
    else
        dump_utmp_file(argv[1]);

    return 0;
}

void dump_utmp_file(char *filename)
{
    struct utmp utmp_record;
    int file_descriptor;

    file_descriptor = open(filename, 0);
    if (file_descriptor == -1)
    {
        perror(filename);
        return;
    }

    while (read(file_descriptor, &utmp_record, sizeof(utmp_record)) == sizeof(utmp_record))
        dump_utmp_record(&utmp_record);

    close(file_descriptor);
}

void dump_utmp_record(struct utmp *record)
{
    printf("%-8.8s ", record->ut_user);
    printf("%-12.12s ", record->ut_line);
    printf("%6d ", record->ut_pid);
    printf("%4d %-12.12s ", record->ut_type, get_utmp_type_name(record->ut_type));
    printf("%12d ", record->ut_time);
    printf("%s ", record->ut_host);
    putchar('\n');
}

char *utmp_type_names[] = {"EMPTY", "RUN_LVL", "BOOT_TIME", "OLD_TIME",
                            "NEW_TIME", "INIT_PROCESS", "LOGIN_PROCESS",
                            "USER_PROCESS", "DEAD_PROCESS", "ACCOUNTING"};

char *get_utmp_type_name(int type_number)
{
    return utmp_type_names[type_number];
}
