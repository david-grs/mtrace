mtrace
======
A very simple wrapper around malloc/free/realloc GNU libc hooks.

Be aware that this class is not *thread-safe*. This is the reason why these hooks are marked as *deprecated* in the glibc ; it is very unlikely that they get removed, but they should not be used in a multithreading environment, as a race condition might occured in the set/get of the hooks.

If you want to hook malloc/free/realloc in a multithreaded environment, you should replace these symbols directly (they are weak symbols in the GNU libc), and not use this class.
