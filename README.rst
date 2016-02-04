Sealang
=======

.. image:: https://travis-ci.org/pybee/sealang.svg?branch=master
    :target: https://travis-ci.org/pybee/sealang

Sealang is an improved set of Python bindings for ``libclang``.

The upstream maintainers of ``libclang`` have not been especially responsive
to patches that address issues like Python 3 compatibility, and the
significant omissions in the API (such as extracting literals and operators).

This package is a fork of the official Python bindings for ``libclang``, patched to correct these problems.

Installation
------------

To compile Sealang, you'll need to:

1. Install LLVM (with clang)
2. Set some environment variables
3. Install Sealang

The exact commands needed will vary between platforms.

OS X
~~~~

Although OS X provides Clang, it doesn't provide all the development headers,
so you'll need to . `Homebrew`_ is the easiest way to do this. Follow the
installation instructions on the Homebrew homepage; once you've got Homebrew
installed, you can run::

    $ brew install llvm --with-clang --with-asan

to get a working install of llvm with clang.

.. _Homebrew: http://brew.sh

Then, you'll need to set the following environment variables::

    $ export LLVM_HOME=/usr/local/opt/llvm
    $ export DYLD_LIBRARY_PATH=$LLVM_HOME/lib

Lastly, you can install Sealang::

    $ pip install sealang

Ubuntu 14.04 (Trusty)
~~~~~~~~~~~~~~~~~~~~~

To compile under Ubuntu 14.04 (Trusty), you'll need to get an updated version
of LLVM::

    $ sudo apt-get install libclang-3.6 clang-3.6 -y

Then, you'll need to set the following environment variables::

    $ export LLVM_HOME=/usr/lib/llvm-3.6
    $ export LD_LIBRARY_PATH=$LLVM_HOME/lib

Lastly, you can install Sealang::

    $ pip install sealang

Other platforms
~~~~~~~~~~~~~~~

The instructions for installing on other platforms should be analogous. If you
develop build instructions for a platform, please `submit a pull request`_.

Usage
-----

Sealang provides a superset of the functionality provided by ``libclang``. Those features are:

* ``Cursor`` objects have 4 additional attributes:

  - ``literal`` - the value of a literal expression. Available on
    IntegerLiteral, FloatingLiteral, StringLiteral, CharacterLiteral, and
    CXXBooleanLiteral nodes.

  - ``operator`` - the printable version of an operator. Only available on
    BinaryOperator, UnaryOperator, and CompoundAssignOperator cursor nodes.

  - ``binary_operator`` - an enumeration value describing a BinaryOperator or
    CompoundAssignOperator node.

  - ``unary_operator`` - an enumeration value describing a UnaryOperator node.

* ``BinaryOperator`` - An enumeration for the types of binary operators:

  - ``BinaryOperator.INVALID``
  - ``BinaryOperator.PTRMEMD``
  - ``BinaryOperator.PTRMEMI``
  - ``BinaryOperator.MUL``
  - ``BinaryOperator.DIV``
  - ``BinaryOperator.REM``
  - ``BinaryOperator.ADD``
  - ``BinaryOperator.SUB``
  - ``BinaryOperator.SHL``
  - ``BinaryOperator.SHR``
  - ``BinaryOperator.LT``
  - ``BinaryOperator.GT``
  - ``BinaryOperator.LE``
  - ``BinaryOperator.GE``
  - ``BinaryOperator.EQ``
  - ``BinaryOperator.NE``
  - ``BinaryOperator.AND``
  - ``BinaryOperator.XOR``
  - ``BinaryOperator.OR``
  - ``BinaryOperator.LAND``
  - ``BinaryOperator.LOR``
  - ``BinaryOperator.ASSIGN``
  - ``BinaryOperator.MULASSIGN``
  - ``BinaryOperator.DIVASSIGN``
  - ``BinaryOperator.REMASSIGN``
  - ``BinaryOperator.ADDASSIGN``
  - ``BinaryOperator.SUBASSIGN``
  - ``BinaryOperator.SHLASSIGN``
  - ``BinaryOperator.SHRASSIGN``
  - ``BinaryOperator.ANDASSIGN``
  - ``BinaryOperator.XORASSIGN``
  - ``BinaryOperator.ORASSIGN``
  - ``BinaryOperator.COMMA``
  - ``BinaryOperator.UNKNOWN``

* ``UnaryOperator`` - An enumeration for the types of binary operators:

  - ``UnaryOperator.INVALID``
  - ``UnaryOperator.POSTINC``
  - ``UnaryOperator.POSTDEC``
  - ``UnaryOperator.PREINC``
  - ``UnaryOperator.PREDEC``
  - ``UnaryOperator.ADDROF``
  - ``UnaryOperator.DEREF``
  - ``UnaryOperator.PLUS``
  - ``UnaryOperator.MINUS``
  - ``UnaryOperator.NOT``
  - ``UnaryOperator.LNOT``
  - ``UnaryOperator.REAL``
  - ``UnaryOperator.IMAG``
  - ``UnaryOperator.EXTENSION``
  - ``UnaryOperator.UNKNOWN``

.. Documentation
.. -------------

.. Documentation for Sealang can be found on `Read The Docs`_.

How it works
------------

Sealang is a bit of a nasty hack. ``libclang`` is a set of C bindings to a C++
API; Python ``ctypes`` are then used to wrap the C API. However, while the C++
API is quite rich, ``libclang`` is less so.

Sealang bridges this gap by providing C wrappers around the C++ calls that
provide the useful functionality. This library of C functions is wrapped up as
a Python C module for delivery purposes - this C module contains no exposed
*Python* objects or methods, but because it's a module, the underlying
compiled `sealang.so` file is easy to find. `ctypes` are then used to expose
the `sealang` wrapper functions;

Internally, Sealang reproduces some minor pieces of the ``libclang`` API;
these are methods (such as the string creation and manipulation methods) that
aren't exposed as symbols for third-party use.

All this functionality is potentially a candidate to be passed upstream to
libclang.

Community
---------

Sealang is part of the `BeeWare suite`_. You can talk to the community through:

* `@pybeeware on Twitter`_

* The `BeeWare Users Mailing list`_, for questions about how to use the
  BeeWare suite.

* The `BeeWare Developers Mailing list`_, for discussing the development of
  new features in the BeeWare suite, and ideas for new tools for the suite.

Contributing
------------

If you experience problems with Sealang, `log them on GitHub`_. If you
want to contribute code, please `fork the code`_ and `submit a pull request`_.

.. _BeeWare suite: http://pybee.org
.. _Read The Docs: http://sealang.readthedocs.org
.. _@pybeeware on Twitter: https://twitter.com/pybeeware
.. _BeeWare Users Mailing list: https://groups.google.com/forum/#!forum/beeware-users
.. _BeeWare Developers Mailing list: https://groups.google.com/forum/#!forum/beeware-developers
.. _log them on Github: https://github.com/pybee/sealang/issues
.. _fork the code: https://github.com/pybee/sealang
.. _submit a pull request: https://github.com/pybee/sealang/pulls
