/* suduo - elevate to root using login_duo */
#include <stdlib.h>
#include <unistd.h>

/* IP address to report to Duo if no SSH_CONNECTION detected */
#define FALLBACK_IP     "0.0.0.0"

/* username for Duo auth */
#define DUO_USER        "root"

/* target user identity for su(1), usually "root" */
#define SU_TARGET_USER  "root"

/* path to su(1) */
#define PROG_SU         "/usr/bin/su"

/* path to login_duo(8) */
#define PROG_LOGIN_DUO  "/usr/local/sbin/login_duo"

/* whether to use pledge(2), unveil(2) -- tech demo, more or less */
#define OPENBSD_HARDEN  1


#if OPENBSD_HARDEN
#define do_unveil(_path, _perm)  \
    do { if (unveil((_path), (_perm)) == -1) { return 250; } } while (0)
#else
#define do_unveil(_path, _perm)
#endif

int main (void) {
    const char* var;    /* buffer for getenv() */
    char* cmdv[8 + 1];  /* max number of args + terminating NULL */
    size_t k;           /* number of args so far (== next free index) */

#if OPENBSD_HARDEN
    if ( pledge ( "stdio rpath id exec unveil", NULL ) != 0 ) {
        return 253;
    }
#endif

    /* make sure to remove SSH_ORIGINAL_COMMAND from env */
    if ( unsetenv ( "SSH_ORIGINAL_COMMAND" ) != 0 ) { return 254; }

    /* fill up cmdv,
     *   login_duo -f <DUO_USER> [-h <FALLBACK_IP>] su -l <SU_TARGET_USER>
     * */
    k = 0;
    cmdv[k++] = PROG_LOGIN_DUO;
    cmdv[k++] = "-f";
    cmdv[k++] = DUO_USER;

    /* set FALLBACK_IP if no ssh connection detected via getenv() */
    var = getenv ( "SSH_CONNECTION" );
    if ( (var == NULL) || (*var == '\0') ) {
        if ( var != NULL ) { unsetenv ( "SSH_CONNECTION" ); }  /* nuke empty var */

        cmdv[k++] = "-h";
        cmdv[k++] = FALLBACK_IP;
    }
    
    /* insert command to be run after successful duo auth */
    cmdv[k++] = PROG_SU;
    cmdv[k++] = "-l";
    cmdv[k++] = SU_TARGET_USER;

    /* terminating NULL ptr for cmdv */
    cmdv[k] = NULL;

    /* set uid/gid, but continue with execv on error */
    if ( issetugid() ) {
        setuid ( 0 );  /* retcode ignored */
        setgid ( 0 );  /* retcode ignored */
    }

    /* exec */
    do_unveil ( PROG_LOGIN_DUO, "x" );
    do_unveil ( PROG_SU, "x" );
    execv ( cmdv[0], cmdv );

    return 255;
}
