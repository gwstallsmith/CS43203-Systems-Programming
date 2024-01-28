/* who2.c  - read /etc/utmp and list info therein
 *         - suppresses empty records
 *         - formats time nicely
 */
#include        <stdio.h>
#include        <unistd.h>
#include        <utmp.h>
#include        <fcntl.h>
#include        <time.h>

#define      SHOWHOST

void showtime(long);
void show_info(struct utmp *, int argc);


int main(int argc, char* argv[])
{

        if(argc > 1) {
            for(int i = 1; i < argc; i++) {
                printf("%s ", argv[i]);
            }
            printf("\n");
        }
        
        struct utmp     utbuf;          /* read info into here */
        int             utmpfd;         /* read from this descriptor */

        if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){
                perror(UTMP_FILE);
                _exit(1);
        }

        while( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf) )
                show_info( &utbuf, argc);
        close(utmpfd);
        return 0;
}
/*
 *      show info()
 *                      displays the contents of the utmp struct
 *                      in human readable form
 *                      * displays nothing if record has no user name
 */
void show_info( struct utmp *utbufp, int argc )
{
        if ( utbufp->ut_type != USER_PROCESS )
                return;

        printf("%-8.8s", utbufp->ut_name);      /* the logname  */
        //printf("%i", argc);
       
        if(argc == 4) {
            printf(" ");                            /* a space      */
            printf("%-8.8s", utbufp->ut_line);      /* the tty      */
            printf(" ");                            /* a space      */
            showtime( utbufp->ut_time );
            
            
        #ifdef SHOWHOST
                if ( utbufp->ut_host[0] != '\0' )
                        printf(" (%s)", utbufp->ut_host);/* the host    */
        #endif
            /* display time */
        }
        printf("\n");                          /* newline      */
}

void showtime( long timeval )
/*
 *      displays time in a format fit for human consumption
 *      uses ctime to build a string then picks parts out of it
 *      Note: %12.12s prints a string 12 chars wide and LIMITS
 *      it to 12chars.
 */
{
        char    *cp;                    /* to hold address of time      */

        cp = ctime(&timeval);           /* convert time to string       */
                                        /* string looks like            */
                                        /* Mon Feb  4 00:46:40 EST 1991 */
                                        /* 0123456789012345.            */
        printf("%12.12s", cp+4 );       /* pick 12 chars from pos 4     */
}