about
-----
h is a library of C programming tools. It's free.  Modify it, contribute, use
it for commercial purposes—-do whatever you want with it.  h has containers,
client/server networking, thread synchronization, PostgreSQL database access,
uuids, config file support, and logging. h was created by Matthew Van Temple.
For more information go to: http://htools.wordpress.com/.

getting started
---------------
+ set up your environment
  + set $H to the directory where you checked out this project (the directory
    containing this file)
  + add $H/bin to $PATH (needed for the build)
+ install
  + OS development headers (stddef.h etc.), and set $H_C_STANDARD_INCLUDE_DIR
    to the place on your OS that includes them
    (for example: /usr/lib/gcc/i486-linux-gnu/4.2.4/include)
  + an ANSI C compiler                  ( http://en.wikipedia.org/wiki/ANSI_C )
  + Doxygen                           ( http://www.stack.nl/~dimitri/doxygen/ )
  + IJG JPEG                                            ( http://www.ijg.org/ )
  + ncurses                            ( http://en.wikipedia.org/wiki/Ncurses )
  + OpenSSL                                         ( http://www.openssl.org/ )
  + POSIX Threads                ( http://en.wikipedia.org/wiki/POSIX_Threads )
  + PostgreSQL                                   ( http://www.postgresql.org/ )
    + compile with --enable-thread-safety
    + set $H_POSTGRESQL_INCLUDE_DIR to the directory containing libpq-fe.h
      (for example: /usr/include/postgresql)
  + rsync                                ( http://en.wikipedia.org/wiki/Rsync )
+ develop and compile
  + "cd $H"
  + "make help"
  + if you want to commit back to the project
    + read and follow the guide below
    + "make release" and "make clean" before committing

formatting
----------
+ use k & r style, except:
  + indent with two spaces
+ use braces around single-statement parts of if-then
+ use "f() {}" (open/close braces on the same line) to show that there is no
  plan to implement this function
+ no whitespace at the end of a line
+ for multiline critical blocks, format like this:
  lock();
  {
      do-stuff();
      do-more-stuff();
  }
  unlock();

structure
---------
+ in a function, use four blocks sepetated by double newlines
  + four blocks
    + assert preconditions and do declarations (with no initialization)
    + initializations (casts, then other initializations)
    + main business
    + assert postconditions and return
  + if the whole function is three lines or less, or the whole function body is
    one statement, don't use double newlines to separate the blocks
+ in a file, place things in this order:
  + #ifndef (if an .h file)
  + #includes
  + #defines
  + enum declarations
  + struct declarations
  + static variable declarations
  + static function declarations
  + function definitions
+ alphabetize functions by name in .h and .c files

terms
-----
+ the highest level directory "h" is called a project
+ the next level directories are called "systems"
+ a .h/.c/.test.c triplet is associated with a "module"

naming
------
+ name modules like project_domain_module (h_core_set)
+ name functions like domain_module_verb or module_verb_noun
  (h_core_set_add_object())
+ create/get/get_as functions
  + a get_x() function returns a value or pointer that the providing object
    manages
  + a get_x_copy() function returns a pointer that the caller manages
  + a get_as() function returns a a value or pointer that the caller manages
    + uuid_get_as_string()
  + a create() function returns a value or pointer that the caller manages
    + string_create_from_uuid()
+ in a container, "replace" means remove, then add; or find, remove, then add
+ ii or jj for naming indexes is prohibited :: use something short like i, j,
  ax, bx, or use something like "object" or "each_object", or "object_index"
+ use "_size" to name a variable/function that indicates the size of something
  including any terminator characters; use "_length" to name a
  variable/function that indicates the number of characters in a string,
  excluding the terminator character
+ if there's one main executable in a system named xyz, name the executable's
  main program source xyz.c
+ name a system's central logical module "system" (with system.h/system.c/
  system.test.c)

logic
-----
+ in a system that has a create() function, always have a destroy() function;
  in a system that has an init() function, always have a free() function and
  always call the free function even if the free function doesn't do anything
  right now...it might someday
+ one return per function, at the end of the function, no guard clauses
+ test every return condition
  + no need to test these unless you want to:
    + h_core_trace(), h_core_trace_exit()
    + h_core_log_enter(), h_core_log_trace()
  + don't test these:
    + get() functions where the value is guaranteed to be there by a
      precondition :: like, it's returning a pointer to, or copy of, something
      that is already guaranteed to be there
    + a h_core_map_find() where a precondition guarantees that that item is
      there...like as the result of an sql query, where you've checked the
      return value of the query() function
  + we're not testing pthread_mutex_[un]lock() right now...
+ don't initialize things until/unless you need to
+ don't use the const keyword
+ use assert()s to express preconditions unless it's a one-line function...no
  need to specify postconditions using assert()s...there are some functions
  that do this, they're from an older time
+ logging
  + use h_core_trace(), h_core_trace_exit(), and h_core_log_trace() when you
    want to report an error :: an error is defined as when you call a function
    and it returns a value that indicates that it didn't do what you wanted it
    to do
    + the only text you pass to a trace message is the name of the function you
      called that returned an error
  + use h_core_log_enter() for everything else
  + prefer h_core_log_enter() and h_core_log_trace() over h_core_trace() and
    h_core_trace_exit()
  + only use h_core_trace_exit() in a file that contains a main()
+ put constants first when using == operator (0 == c)
+ sizeof(char) is always 1 :: don't multiply anything by sizeof(char)
+ no comments in completed code--if you need comments it's because the
  identifiers are named improperly, except:
  + doxygen comments
+ put a default: block in every switch statement, followed by at least a break;
+ when referring to paths/urls/directories, always start with a slash, never
  end with one
+ generally, don't use straight int's, use short or long
  + (might be nice to actually use uint32, that sort of thing...I mean just to
    make it clear what's happening in network-serialization type situations)
+ generally, do one thing per line
  + generally, don't use the tertiary operator
  + generally, don't use the comma operator
  + generally, set a variable to the return value of a function, then use that
    variable in the call to another function, rather than nesting function
    calls
+ generally, code for what you want to happen, not what you don't want to
  happen
