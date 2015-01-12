#ifndef XP_SLEEP_H
# define XP_SLEEP_H

# ifdef _WINDOWS
#  include <windows.h>
# else
#  include <unistd.h>
#  define Sleep(x) (usleep((x) * 1000))
# endif

#endif
