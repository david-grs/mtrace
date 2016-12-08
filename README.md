mtrace
======
A C++ wrapper around malloc/free/realloc GNU libc hooks.


Example
-------
```
    mtrace<malloc_chrono> mt;

    {
        auto uptr = std::make_unique<int>(5);
        std::cout << "after unique_ptr ctor: malloc=" << std::get<0>(mt).malloc_time().count() << "ns "
                  << " free=" << std::get<0>(mt).free_time().count() << "ns" << std::endl;
    }

    std::cout << "after unique_ptr dtor: malloc=" << std::get<0>(mt).malloc_time().count() << "ns "
              << " free=" << std::get<0>(mt).free_time().count() << "ns" << std::endl;
```

Output:

```
    after unique_ptr ctor: malloc=5100ns free=0ns
    after unique_ptr dtor: malloc=5100ns free=1131ns
```


Note
----
Be aware that this class is not *thread-safe*. This is the reason why these hooks are marked as *deprecated* in the glibc ; it is very unlikely that they get removed, but they should not be used in a multithreading environment, as a race condition might occured in the set/get of the hooks.

If you want to hook malloc/free/realloc in a multithreaded environment, you should replace these symbols directly (they are weak symbols in the GNU libc), and not use this class.
