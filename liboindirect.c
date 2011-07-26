/*
  Bugs: avati@gluster.com
*/

#include <dlfcn.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>


#define __cons __attribute__((constructor))

#define TRAP(func, params)                                              \
        evil_##func params;                                             \
        static int (*real_##func) params;                               \
        static void set_real_##func (void) __cons;                      \
        static void set_real_##func (void)                              \
        { real_##func = dlsym (RTLD_NEXT, #func); }                     \
        int __REDIRECT (evil_##func, params, func);                     \
        int evil_##func params


int loi_debug = 0;

int
dbg (const char *fmt, ...)
{
        int     ret = 0;
        va_list ap;

        if (!loi_debug)
                return 0;

        va_start (ap, fmt);
        ret = vfprintf (stderr, fmt, ap);
        va_end (ap);

        return ret;
}


int
err (const char *fmt, ...)
{
        int     ret = 0;
        va_list ap;

        va_start (ap, fmt);
        ret = vfprintf (stderr, fmt, ap);
        va_end (ap);

        return ret;
}


static void
dbg_init ()
{
        if (getenv ("LOI_DEBUG"))
                loi_debug = 1;
}



int
TRAP (open, (const char *path, int flags, mode_t mode))
{
        int ret;

        ret = real_open (path, flags, mode);

        if (ret >= 0)
                return ret;

        if (ret == -1 && errno == EINVAL && (flags & O_DIRECT)) {
                flags &= ~O_DIRECT;
                flags |= O_SYNC; /* instead */
        }

        ret = real_open (path, flags, mode);

        return ret;
}


int
TRAP (open64, (const char *path, int flags, mode_t mode))
{
        int ret;

        ret = real_open64 (path, flags, mode);

        if (ret >= 0)
                return ret;

        if (ret == -1 && errno == EINVAL && (flags & O_DIRECT)) {
                flags &= ~O_DIRECT;
                flags |= O_SYNC; /* instead */
        }

        ret = real_open64 (path, flags, mode);

        return ret;
}


int
TRAP (openat, (int dirfd, const char *path, int flags, mode_t mode))
{
        int ret;

        ret = real_openat (dirfd, path, flags, mode);

        if (ret >= 0)
                return ret;

        if (ret == -1 && errno == EINVAL && (flags & O_DIRECT)) {
                flags &= ~O_DIRECT;
                flags |= O_SYNC; /* instead */
        }

        ret = real_openat (dirfd, path, flags, mode);

        return ret;

}



static void loi_init (void) __attribute__((constructor));


static void
loi_init (void)
{
        dbg_init ();

        return;
}

