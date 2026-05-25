Installation
============

``wlcov`` is distributed as source code.  A complete local installation builds
the C executable, the static library, and the Cython extension module.

Prerequisites
-------------

Required build tools:

* a POSIX-like shell environment;
* ``make``;
* a C compiler such as ``gcc``;
* the `GNU Scientific Library`_ headers and libraries;
* Python 3 with ``pip``;
* ``numpy`` and ``Cython`` for the Python wrapper.

Recommended documentation tools:

* ``sphinx``;
* ``sphinx-rtd-theme``;
* ``sphinx-prompt``.

Build From Source
-----------------

On Debian or Ubuntu systems with GSL installed in system locations, a typical
build is:

.. prompt:: bash

   python3 -m pip install --user numpy Cython
   make clean
   make PYTHON=python3 GSL_INCL="-I/usr/include" \
        GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all

The build creates:

* ``wlcov``: command-line executable;
* ``libwlcov.a``: static C library used by the wrapper;
* ``wlcovpy``: importable Cython extension installed by ``pip``.

Custom GSL Installations
------------------------

If GSL is installed outside standard compiler paths, either edit
``Makefile_machine`` or override the Makefile variables at build time:

.. prompt:: bash

   export GSL_DIR="$HOME/local/gsl"
   export GSL_INCLUDE_DIR="$GSL_DIR/include"
   export GSL_LIBRARY_DIR="$GSL_DIR/lib"
   make PYTHON=python3 \
        GSL_INCL="-I${GSL_INCLUDE_DIR}" \
        GSL_LIBS="-L${GSL_LIBRARY_DIR} -lgsl -lgslcblas" all

The Python extension honors ``CC``, ``GSL_DIR``, ``GSL_INCLUDE_DIR``, and
``GSL_LIBRARY_DIR``.  The Makefile still needs ``GSL_INCL`` and ``GSL_LIBS`` if
GSL is not discoverable by the compiler and linker.

OpenMP Builds
-------------

OpenMP support is controlled in ``Makefile_settings`` with
``OPENMPMACHINE = 1``.  It can also be enabled from the command line:

.. prompt:: bash

   make clean
   make PYTHON=python3 OPENMPMACHINE=1 \
        GSL_INCL="-I/usr/include" \
        GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all

Set ``OMP_NUM_THREADS`` or pass ``numberThreads=<N>`` at runtime.  When
``OPENMPMACHINE = 0``, ``numberThreads`` is accepted for API consistency but
does not change execution.

Verify the Installation
-----------------------

Check the command-line interface:

.. prompt:: bash

   ./wlcov --help

Run the compact C example:

.. prompt:: bash

   ./wlcov inputfile=tests/input/Cls_ep2.txt rootDir=Output_check \
      ellmax=25 ppp=4 verbose=0 verbose_log=0

Check the Python wrapper:

.. prompt:: bash

   cd tests
   python3 python/kappa_cov.py --fnamePS input/Cls_ep2.txt \
      --outdir Output_python --ellmax 25 --ppp 4

Build the Documentation
-----------------------

The documentation follows the same Sphinx builder convention used by
``cTreeBalls/docs``:

.. prompt:: bash

   python3 -m pip install --user -r docs/requirements.txt
   cd docs
   make html
   make man
   make latexpdf

The generated artifacts are:

* ``docs/_build/html/index.html``;
* ``docs/_build/man/wlcov.1``;
* ``docs/_build/latex/wlcov.pdf``.

``make latexpdf`` requires a LaTeX installation with ``latexmk`` and
``pdflatex`` available on ``PATH``.
