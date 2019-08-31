/* load LOCALCONFIG */
#ifdef LOCALCONFIG
#define __get_header(h) #h
#define _get_header(h)  __get_header(h)

#define __LOCALCONFIG _get_header(LOCALCONFIG)
#include __LOCALCONFIG

#undef _get_header
#undef __get_header
#endif


/* IP address to report to Duo if no SSH_CONNECTION detected */
#ifndef FALLBACK_IP
#define FALLBACK_IP     "0.0.0.0"
#endif

/* username for Duo auth */
#ifndef DUO_USER
#define DUO_USER        "root"
#endif

/* target user identity for su(1), usually "root" */
#ifndef SU_TARGET_USER
#define SU_TARGET_USER  "root"
#endif

/* path to su(1) */
#ifndef PROG_SU
#define PROG_SU         "/usr/bin/su"
#endif

/* path to login_duo(8) */
#ifndef PROG_LOGIN_DUO
#define PROG_LOGIN_DUO  "/usr/local/sbin/login_duo"
#endif

/* shell for su -s <SHELL>, optional */
//#define SU_SHELL        "/bin/sh"


/* whether to use pledge(2) -- tech demo, more or less */
#ifndef OPENBSD_HARDEN
#define OPENBSD_HARDEN  0
#endif
