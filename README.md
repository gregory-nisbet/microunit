# microunit

A small unit test framework for C and C++. Microunit produces a strict subset
of TAP.  It does not number the tests in its output because it does not
guarantee that the tests are run in a specific order.

The test API works by returning C strings in the event of failure. A null
pointer indicates that a single test or test suite has finished successfully.
The C strings will normally be string literals. Microtest makes no attempt to
manage the lifetimes of the string error objects that are passed to it. If you
need to pass a dynamically generated string, as of right now you must leak the
memory.

In the future I might add some way of allocating string memory from a pool
owned by the microunit library.
