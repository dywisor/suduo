/* suduo - elevate to root using login_duo */
#include <stdlib.h>
#include <unistd.h>

#include "config.h"


int main (void) {
    static const unsigned LOGIN_DUO_CMDV_SIZE = (
        0

        + 3     /* PROG_LOGIN_DUO "-f" DUO_USER */

#ifdef FALLBACK_IP
        + 2     /* -h FALLBACK_IP */
#endif

        + 2     /* PROG_SU -l */

#ifdef SU_SHELL
        + 2     /* -s SU_SHELL */
#endif

#ifdef SU_TARGET_USER
        + 1     /* SU_TARGET_USER */
#endif

    );

    const char* var;    /* buffer for getenv() */
    char* cmdv[LOGIN_DUO_CMDV_SIZE + 1];  /* + terminating NULL */
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

#ifdef FALLBACK_IP
    /* set FALLBACK_IP if no ssh connection detected via getenv() */
    var = getenv ( "SSH_CONNECTION" );
    if ( (var == NULL) || (*var == '\0') ) {
        if ( var != NULL ) { unsetenv ( "SSH_CONNECTION" ); }  /* nuke empty var */

        cmdv[k++] = "-h";
        cmdv[k++] = FALLBACK_IP;
    }
#endif

    /* insert command to be run after successful duo auth */
    cmdv[k++] = PROG_SU;

#ifdef SU_SHELL
    cmdv[k++] = "-s";
    cmdv[k++] = SU_SHELL;
#endif
    cmdv[k++] = "-l";

#ifdef SU_TARGET_USER
    cmdv[k++] = SU_TARGET_USER;
#endif

    /* terminating NULL ptr for cmdv */
    cmdv[k] = NULL;

    /* set uid/gid, but continue with execv on error */
    if ( setuid ( 0 ) != 0 ) { return 253; }
    if ( setgid ( 0 ) != 0 ) { return 253; }

    /* exec */
    execv ( cmdv[0], cmdv );

    return 255;
}
