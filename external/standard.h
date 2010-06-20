#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <libpq-fe.h>
#include <math.h>

#include <ncurses.h>
#undef false
#undef true

#include <openssl/rand.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <jpeglib.h>

#ifdef __NetBSD__
#include <uuid.h>
#endif

#ifdef __linux__
#include <uuid/uuid.h>
#endif
