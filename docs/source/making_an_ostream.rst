***********************
Making an ostream Class
***********************

It is common C++ practice to make an object of type ``T`` printable by
overloading the ``std::ostream& operator<<(std::ostream&, T)`` function. Thus if
you want to do special printing a common strategy is to implement a class that
derives from ``std::ostream``. Being blunt it's quite hard to find good
documentation relating to how to do this.

Overview
========

So in the opening we said we want to inherit from ``std::ostream``. This is
actually the easy part because that always looks like:

.. code-block:: c++

   class MyStream : public MyBuffer, public std::ostream {
   public:
       MyStream() : MyBuffer(), std::ostream(this) {}
   };

More specifically ``std::ostream`` has no virtual functions so you can't change
it's behavior that way. Rather what you do is define a new buffer object. Buffer
objects are more or less PIMPLs for the stream. The  ``std::ostream`` ctor takes
a pointer to the buffer it is supposed to use. Arguably the cleanest way to
store the state of the buffer in the class and ensure the ``std::ostream`` has
a valid pointer to it is to use multiple inheritance. This is because base
classes are initialized before members of derived classes. Hence, if we put the
buffer in the ``MyStream`` class it wouldn't be ready when we need to initialize
the ``std::ostream`` class. By using multiple inheritance we get to initialize
the base classes in the order they are declared, so we first inherit from our
buffer (which is currently hidden behind behind the opaque type ``MyBuffer``)
and then provide the ``this`` pointer to ``std::ostream``.

To get our stream to do what we want we have to write our own buffer class
that derives from ``std::basic_streambuf<T>`` where ``T`` is the type used to
hold the characters of the stream. Typically you want this to either be ``char``
or ``wchar_t`` and the STL provides typedefs for you (``std::streambuf`` and
``std::wstreambuf`` respectively). Our plan is thus to define a class
``MyBuffer`` that derives from``std::basic_streambuf<T>`` and properly overrides
the virtual functions. One should note that ``std::ostream`` is actually a
similar typedef of ``std::basic_ostream<char>`` hence it's necessary to enusre
that the buffer class we write has the same template type parameter.

This is where things get tricky, as the documentation for
the virtual functions is a bit archaic. If you're looking at something like
`cplusplus.com <http://www.cplusplus.com/reference/streambuf/basic_streambuf/>`_
you'll hear about "controlled output sequence" (which is where your buffer
object is writing to, think the screen or a file), "controlled input sequence"
(which is where your buffer object is getting data from, think standard in or a
file).

std::basic_streambuf
====================

This is class that actually does all the work for a stream. Internally it has
two buffers an input buffer and an output buffer. When you use the stream's
``operator<<()`` funct, the data provided to the function is placed in the
output buffer until an overflow occurs (*i.e.*, there's no more room in the
output buffer). The buffer is then responsible for making additional room in the
buffer. The input buffer is tied to the ``operator>>()`` function and works
similarly. Data is read from the input buffer until there is no data left to
read, at which point an underflow occurs.

The internal buffers of the ``std::basic_streambuf`` class are described by six
pointers, which are referred to by the names of the member functions for
retrieving the pointer:

- ``eback`` a pointer to the beginning of the input buffer
- ``gptr()`` the current position in the input buffer
- ``egptr()`` pointer to just past the end of the input buffer
- ``pbase()`` beginning of the output buffer
- ``pptr()`` current position in the output buffer
- ``epptr()`` pointer to just past the end of the output buffer

If you're trying to make sense of the names ``gptr()`` stands for the
"get pointer" and ``pptr()`` stands for the "put pointer". Those two seem to be
rigorously defined. I can't find definitions for the others, but my guesses
(which help me mnemonically at least) are that the "e" in the "e" prefixed
versions (``egptr()`` and ``epptr()``) stands for "end", ``pbase`` stands for
the "base" of the put array, and I'm really not sure about ``eback``, but by
process of elimination it must be the start of the input buffer...

By default all of these pointers are null, which means that every write/read
will overflow/underflow. Your class needs to maintain these pointers if it
actually has an internal buffer. To set the output pointers use the ``setp``
function and to set the input pointers use ``setg``. The input to these
functions are the new beginning and end pointers; the current position will
automatically be set to the beginning. Experience suggests that if you override
``seekoff`` and ``seekpos``, then you only need to worry about setting pointers
in the event of an overflow/underflow.

Anyways, internally ``operator<<`` calls ``sputc`` for each character in the
input data. ``sputc`` looks something like:

.. code-block:: c++

   int_type sputc(char_type c) {
       int_type ic = traits_type::to_int_type(c); //Turn c into an integer type
       if( !pptr() || pptr() == epptr() ) //overflow
               return overflow(ic);
       *pptr() = c;
       pbump(1); //advances the put pointer by 1
       return ic:
   }

Basically ``sputc`` takes care of writing characters to the buffer, one at a
time, until it can't. When it can't it defers to the ``overflow`` function. By
default the ``overflow`` function just returns ``traits_type::eof()``, which
signals failure to handle the overflow and will crash your program. If you want
your buffer class to be an output buffer, then you need to override
``overflow``. As a slight aside, ``underflow`` works similarly by default. So if
you only want your buffer to be an output buffer, the default behavior will
correctly err if someone tries to read from it.

Writing one character at a time is inefficient. This is why
``std::basic_streambuf`` also has ``sputn``. ``sputn``'s signature is:

.. code-block:: c++

   streamsize sputn(const char_type& s, streamsize n);

 When called, ``sputn`` attempts to write at most ``n`` characters from the
stream which starts at ``s``. It returns the number of characters actually
written (if an overflow occurs then only some of the ``n`` characters are
written). ``sputn`` basically just calls ``xsputn``, which is a virtual member
function that can be overriden by your class. By default, ``xsputn`` just calls
``sputc``, ``n`` times, stopping if any of the calls to ``sputc`` return
``traits_type::eof()``.

For writing data to a buffer ``overflow`` and ``xsputn`` are the major virtual
functions to be aware of. Depending on how your buffer works you also want to be
aware of:

- ``sync`` called when the user wants to force a flush. By default this function
  does nothing. So if your buffer does not automatically write its data to its
  final destination you need to override this function.
- ``seekoff`` called to move either ``gptr()`` or ``pptr()`` to a new location,
  with the location specified relative to the beginning, current position, or
  end of the respective internal buffer. Default behavior is to do nothing and
  return -1.
- ``seekpos`` similar to ``seekpos`` except it moves the pointer to the
  specified absolute position.

The following sections provide more details on the virtual functions that are
relevant for writing a custom output buffer.

xsputn
======

If your buffer leverages an existing ``std::ostream`` or ``std::streambf``, this
is the function you want to override. It's signature is:

.. code-block:: c++

   std::streamsize xsputn(const char* s, std::streamsize n)

It takes a pointer to an array of characters and the length of that array. Your
buffer class's implementation of this function should write as many of the
provided characters as possible to the output. After writing either the entire
array, or as many characters as it can, your implementation should return the
nuumber of characters it actually wrote.

The base class provides a default implementation, which just calls ``sputc``
until ``sputc`` returns ``traits_type::eof()`` (hence signaling an error) or all
characters have been written. Note that ``sputc`` works by calling ``overflow``
so if you choose not to override this function, you will need to override
``overflow``.


overflow
========

``overflow`` is called when the ``pptr()`` is no longer valid. Its signature is:

.. code-block:: c++

   int_type overflow(int_type c);

The input value, ``c``, is the character which could not fit in the internal
output buffer. Your implementation is allowed to make more room in the internal
output buffer (if you do this you need to adjust, ``pbase()``, ``pptr()``, and
``epptr()``). The return is c, if ``overflow`` was successful or
``traits_type::eof()`` if an error occurred.


synch
=====

If a user calls flush on the ``std::ostream`` object it ultimately will call
``synch``. ``synch``'s signature is:

.. code-block:: c++

   int synch()

It returns 0 if successful and -1 if it fails. Your implementation should dump
whatever's in the internal output buffer to the output and reset the pointers.

seekoff
=======

.. code-block:: c++

   pos_type seekoff(off_type off,
                    ios_base::seekdir way,
                    ios_base::openmode which)

seekpos
=======

.. code-block:: c++

   pos_type seekpos(pos_type pos, ios_base::openmode which)
