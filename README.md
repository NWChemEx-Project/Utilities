Utilities Repository
======================

The C++ standard library as well as the STL are great for providing many of the
features one needs to write a code.  However, they leave much to be desired.
This library is intended to be our "Boost" in that it's a series of 
extensions to the standard libraries for functionality we commonly want.

At the moment this functionality can be expressed as falling into one of three
sub libraries:

- IterTools: Classes designed to make iteration easier.  This includes base
             classes for writing your own iterators as well as classes for
             iterating over common situations such as:  
             - All permutations of a sequence  
             - All combinations of a sequence       
- Mathematician : Math functionality beyond that of the STL including:  
  - Binomial coefficients
  - Multinomial coefficients
- TypeTraits : Structs to make meta-template programming easier
  - Extended type intraspection (*i.e.* determining if a type has a member)

Building Utilities
--------------------

Utilities piggy-backs off of CMakeBuild so see 
[here](CMakeBuild/dox/Building.md) for instructions on building this library.
