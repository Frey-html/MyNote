In the context of garbage collection, a **root reference** is any reference that is directly accessible by the application and serves as a starting point for the garbage collector to identify live objects. Root references typically include:

1. **Local Variables**: Variables in the current stack frame of executing methods.
2. **Static Fields**: Fields that belong to classes and are not tied to instances.
3. **Active Threads**: References held by currently executing threads.
4. **Global Objects**: Objects referenced by global variables or singletons.

The garbage collector uses these root references to traverse the object graph, marking all reachable objects. Anything not marked as reachable from the roots can be collected.