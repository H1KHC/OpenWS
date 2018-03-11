Waiting for this issue fixed to implement the below function(Found [here](http://forum.lwjgl.org/index.php?topic=5836.msg31107#msg31107)):

- (A) window/context creation
- (B) running the event loop that dispatches events for the window
- (C) making the context current in a thread

Some restrictions on different platform:
- Windows:
    - (A) and (B) must happen on the same thread.
    - It doesn't have to be the main thread.
    - (C) can happen on any thread.
    - But no two threads could have the same context.

- Linux:
    - You can have (A), (B) and (C) on any thread.

- Mac OS X:
    - (A) and (B) must both happen on main thread (thread 0).
    - Again, (C) can happen on any thread.


Function:

- [ ] wsWaitForWindowClose(int windowID)
    - [ ] create a seperate thread for each glfw window(impossible for mac now)
    - [ ] add a global signal varible (like windows' Event), to notify all waiting threads when one thread is closed
