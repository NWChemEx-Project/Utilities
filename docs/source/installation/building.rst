Building the library
====================

Utilities is built using `CPP <https://github.com
/CMakePackagingProject/CMakePackagingProject.git>`_.
Assuming you have already installed CPP,
and you are willing to let cmake build the dependencies (Catch2), then the
following will suffice to build:

.. code-block:: bash

   git clone https://github.com/NWChemEx-Project/Utilities.git
   cd Utilities
   cmake -H. \
         -Bbuild \
         -DCMAKE_PREFIX_PATH=<where/you/installed/CPP> \
         -DCMAKE_INSTALL_PREFIX=<where/you/want/to/install/Utilities>
   cd build
   cmake --build .
   make
   #May need to run as an admin depending on where you are installing Utilities to
   make install

Note that the configure will appear to hang when it gets to Catch2. 
This is because it is building `Catch2 <https://github.com/catchorg/Catch2>`_
testing framework. Building of Catch2 can be 
avoided by disabling tests (*i.e.*, passing ``-DBUILD_TESTS=OFF`` to the first
invocation of ``cmake``) or by providing CMake with an already built version of
Catch2 by passing ``-DCatch2_ROOT=/path/to/catch2`` to the first invocation of 
``cmake``.

For finer-grained control over the build we direct the reader to the more
thorough CPP build instructions located `here <https://cmakepackagingproject
.readthedocs.io/en/latest/end_user/quick_start.html>`_ and note that Utilities
depends on `Catch2 <https://github.com/catchorg/Catch2>`_ for testing.

Building the documentation
==========================

You need to install Doxygen and the following Python packages (all can be
installed with ``pip``):

- sphinx_rtd_theme (The Read-The-Docs theme for sphinx)
- sphinx (The thing that makes the documentation)
- breathe (Dependency of exhale, may get installed by it)
- exhale (The thing that turns Doxygen output into ReST)

You can build the documentation with ``make html`` within ``docs`` directory.
You can view the documentation by opening `docs/build/html/index.html` on your
browser.

