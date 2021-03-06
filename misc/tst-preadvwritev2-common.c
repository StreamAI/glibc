/* Common function for preadv2 and pwritev2 tests.
   Copyright (C) 2017-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <limits.h>
#include <support/check.h>

static void
do_test_with_invalid_flags (void)
{
#ifndef RWF_HIPRI
# define RWF_HIPRI 0
#endif
#ifndef RWF_DSYNC
# define RWF_DSYNC 0
#endif
#ifndef RWF_SYNC
# define RWF_SYNC 0
#endif
#ifndef RWF_NOWAIT
# define RWF_NOWAIT 0
#endif
#ifndef RWF_APPEND
# define RWF_APPEND 0
#endif
#define RWF_SUPPORTED	(RWF_HIPRI | RWF_DSYNC | RWF_SYNC | RWF_NOWAIT \
			 | RWF_APPEND)
  /* Set the next bit from the mask of all supported flags.  */
  int invalid_flag = RWF_SUPPORTED != 0 ? __builtin_clz (RWF_SUPPORTED) : 2;
  invalid_flag = 0x1 << ((sizeof (int) * CHAR_BIT) - invalid_flag);

  char buf[32];
  const struct iovec vec = { .iov_base = buf, .iov_len = sizeof (buf) };
  if (preadv2 (temp_fd, &vec, 1, 0, invalid_flag) != -1)
    FAIL_EXIT1 ("preadv2 did not fail with an invalid flag");
  if (errno != ENOTSUP)
    FAIL_EXIT1 ("preadv2 failure did not set errno to ENOTSUP (%d)", errno);

  /* This might fail for compat syscall (32 bits running on 64 bits kernel)
     due a kernel issue.  */
  if (pwritev2 (temp_fd, &vec, 1, 0, invalid_flag) != -1)
    FAIL_EXIT1 ("pwritev2 did not fail with an invalid flag");
  if (errno != ENOTSUP)
    FAIL_EXIT1 ("pwritev2 failure did not set errno to ENOTSUP (%d)", errno);
}
