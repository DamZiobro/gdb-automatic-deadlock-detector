
gdb-automatic-deadlock-detector
=======

Script adds new GDB command which automatically detects C/C++ thread lockups and deadlocks in GDB debugger. 

**It automates process of deadlock detection described in this instruction**:
https://en.wikibooks.org/wiki/Linux_Applications_Debugging_Techniques/Deadlocks

Automation is written as GDB Python script based on Python GBD API: https://sourceware.org/gdb/onlinedocs/gdb/Python-API.html

**This script adds new 'blocked' command to GDB**. 'blocked' command shows which threads 
are blocking other threads (or are deadlocked each other) ex.
```
(gdb) blocked
********************************************************************************
Displaying blocking threads using 'blocked' command
Thread: 7960 waits for thread: 7959 AND DEADLOCKED
Thread: 7959 waits for thread: 7960 AND DEADLOCKED
********************************************************************************
```
It means that thread 7960 waits for mutex holded by thread 7959 AND thread 7959 
waits for mutex holded by thread 7960 - so threas 7959 and 7960 are deadlocked


Preparing sample deadlocked app
-----
1. Compile sample application containing thread deadlock.
```
gcc -pthread deadlockExample.c -o deadlockExample
```
2. Make sure that your Linux OS can dump core files from the processes. Set it
   up by:
```
ulimit -c unlimited
```
3. Run compiled application 
```
./deadlockExample
```
4. Notice that this application stucks (because of deadlock). Run another
   terminal and send SIGABRT signal (signal nr 6) to the running app:
```
kill -6 $(pidof deadlockExample)
```
5. You should have core file outputed to your PWD dir.
6. Also you need to have your GDB compiled with Python support. Check whether
   you can run 'python' command in your GDB. If not the recompile your GDB 
   using this instruction: https://askubuntu.com/questions/513626/cannot-compile-gdb7-8-with-python-support

Usage
-----
1. See content of the 'gdbcommands' file - it contains commands wilchi will be 
   invoked in GDB to run this to detect deadlock automatically 
   (**notice import our Python script gdbDisplayLockedThreads and invoking 'blocked' command**):
```
python 
import sys
sys.path.append('.')
import gdbDisplayLockedThreads
end 
thread apply all bt 
blocked
```

2. Run gdb and invoke commands from 'gdbcommands' file automatically:
```
gdb -c core ./deadlockExample -x ./gdbcommands -batch
```

3. **Output shows all backtraces of threads from deadlockExample followed by additional info of 
   which threads are waiting for mutexes holding by other threads** ex.
```
Thread 3 (Thread 0x7efc4958f700 (LWP 7960))
  #0 __lll_lock_wait () at ../sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
  #1 0x00007efc4a164cfd in __GI___pthread_mutex_lock (mutex=0x601300 <write_mutex>) at ../nptl/pthread_mutex_lock.c:80
  #2 0x0000000000400aae in readTest ()
  #3 0x00007efc4a1626aa in start_thread (arg=0x7efc4958f700) at pthread_create.c:333
  #4 0x00007efc49e97eed in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:109

Thread 2 (Thread 0x7efc49d90700 (LWP 7959)):
  #0 __lll_lock_wait () at ../sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
  #1 0x00007efc4a164cfd in __GI___pthread_mutex_lock (mutex=0x6012c0 <read_mutex>) at ../nptl/pthread_mutex_lock.c:80
  #2 0x0000000000400a00 in writeTest ()
  #3 0x00007efc4a1626aa in start_thread (arg=0x7efc49d90700) at pthread_create.c:333
  #4 0x00007efc49e97eed in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:109
 
Thread 1 (Thread 0x7efc4a564700 (LWP 7958)):
  #0 0x00007efc4a1638ed in pthread_join (threadid=139622035818960, thread_return=0x0) at pthread_join.c:90
  #1 0x0000000000400b95 in main ()

********************************************************************************
Displaying blocking threads using 'blocked' command
Thread: 7960 waits for thread: 7959 AND DEADLOCKED
Thread: 7959 waits for thread: 7960 AND DEADLOCKED
********************************************************************************
```

It means that thread 7960 waits for mutex holded by thread 7959 AND thread 7959 
waits for mutex holded by thread 7960 - so threas 7959 and 7960 are deadlocked

4. Now you can detect deadlock for any core using this quick gdb command from
   point 2 or 'blocked' command from gdb.

Enjoy your deadlock detections !
-----
