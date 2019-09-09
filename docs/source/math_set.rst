MathSet Developer Notes
=======================

The ``MathSet`` class models an ordered set. It natively (as opposed to free
functions) supports set-like operations such as union, intersection, difference,
and slicing. The notes on this page are meant primarily for developers wanting
to extend the ``MathSet`` class and/or understand how it works.

Design Points
-------------

The ``MathSet`` class was primarily developed to serve as the backend container
for physics concepts that are:

- ordered set-like,
- (possibly) nested set-like

Additionally we require that the resulting object is reasonably efficient. In
particular this means:

- elements are stored contiguously
- nested sets are flattened

Elements
--------

Elements in a ``MathSet`` are treated as immutable. This makes it easier to
reason about correctness and corner cases when we rely on more exotic
implementations. must be:

- copyable


Notes on Views
--------------

For efficiency most ``MathSet`` operations return views of the underlying
containers. For example taking the set-difference of two sets:

.. code-block:: C++

   MathSet<int> s0{1, 2, 3}, s1{2, 4, 6};
   const auto& s2 = s0 - s1; // s2 == {1, 3}

returns a read-only reference to a view of ``s0``. More specifically it is the
case that ``&s2[0] == &s0[0]`` and ``&s2[1] == &s0[2]``. It is always the case
that operations that return views do so by read-only reference. This ensures
that if the user wants to possibly modify the resulting set they need to make a
copy. The copy breaks the aliasing and avoids potential ambiguities related to
assignment.

Generally speaking the intent of the assignment operation on the view is usually
clear. For example, in the following, invalid, code:

.. code-block:: C++

   MathSet<int> s0{1, 2, 3}, s1{2, 4, 6};
   auto& s2 = s0 - s1; // s2 == {1, 3}
   s2[0] = 4;

the intent is likely to make ``s2 == {4, 3}``, whereas in

.. code-block:: C++

   MathSet<int> s0{1, 2, 3}, s1{2, 4, 6};
   auto& s2 = s0 - s1; // s2 == {1, 3}
   s2 = s1;

it seems safe to say that the intent is to make ``s2 == {2, 4, 6}``. The
ambiguity arises when we consider the consequences of the assignment on the
parent set (*i.e.*, ``s0``). There are at least three reasonable user
expectations:

1. The aliasing should break. In this case ``s0 == {1, 2, 3}`` for both code
   examples. In the second example there would additionally be some ambiguity
   regarding whether ``s2`` should alias or copy ``s1``.

2. ``s0`` is a "universe" (set of all possible elements) and thus the additional
   elements from the assignment should be added to ``s0``. Here
   ``s0 == {1, 2, 3, 4}`` for the first code example and
   ``s0 == {1, 2, 3, 4, 6}`` for the second example.

3. The view is to be taken literally and the elements of ``s0`` are to be
   overridden. For the first code example that means ``s0 == {4, 2, 3}`` and
   for the second that means ``s0 == {4, 2, 6}`` (with further ambiguity in the
   order).

It is our view that breaking aliasing should be done by copying and that the
modifications to the parent set should occur through the parent set (where they
are perfectly well-defined).

Notes on Nested Sets
--------------------

An interesting use case is when we have sets of sets. Conceptually this looks
something like: ``s0 = {{1, 2}, {3, 4}, {4, 5}}``. For performance we want to
flatten the internal sets so that the elements are stored contiguously (this
will always be possible because each element of the outer set must have the same
type). When we flatten the above set it gets stored in memory like:
``s0 = [1, 2, 3, 4, 4, 5]``. When we return an element from ``s0`` it holds a
view of ``s0``. Specifically it holds the starting and stopping indices for that
element (for example the second element of ``s0`` would hold now that it starts
at element 2 and stops before element 4). Since we return the elements as views
we run into the same problem as we did in the last section. Here however one
could argue that the user did not want them to be "views", so when you modify
say the second element of ``s0`` you expect ``s0`` to to be modified as well. At
the moment we treat the elements of a MathSet as immutable so you can not edit
them despite this being well defined.

Other Thoughts
--------------

- Could create a binary view, which would be a view of two MathSet instances
  simultaneously.
- Could create a non-flattened nested set PIMPL.
