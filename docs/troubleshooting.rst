Troubleshooting and Common Errors
=================================

``gsl_sf_bessel_J0`` or ``gsl_sf_bessel_Jn`` Undefined at Link Time
-------------------------------------------------------------------

Cause:
    GSL libraries were not linked after the object files, or ``GSL_LIBS`` does
    not point to a valid GSL installation.

Fix:
    Use the current Makefile and pass GSL paths explicitly:

    .. prompt:: bash

       make clean
       make PYTHON=python3 GSL_INCL="-I/usr/include" \
            GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all

``python: command not found``
-----------------------------

Cause:
    The Makefile default uses ``PYTHON ?= python``.  Some systems only provide
    ``python3``.

Fix:
    Pass ``PYTHON=python3`` to ``make``.

``No module named Cython``
--------------------------

Cause:
    The wrapper build imports Cython from ``setup.py``.

Fix:
    Install Python build dependencies before running ``make all``:

    .. prompt:: bash

       python3 -m pip install --user numpy Cython

Undefined ``SetNumberThreads`` or ``get_nthreads`` Symbols
----------------------------------------------------------

Cause:
    Older builds did not compile the thread-control and PXD helper symbols
    required by the Cython wrapper.

Fix:
    Rebuild from a checkout containing the current fixes:

    .. prompt:: bash

       make clean
       make PYTHON=python3 GSL_INCL="-I/usr/include" \
            GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all

Parameter File Tags Not Recognized
----------------------------------

Cause:
    Parameter files must use full parameter names.  Aliases such as ``root`` or
    ``in`` are command-line conveniences only.

Fix:
    Use names such as ``rootDir`` and ``inputfile`` in parameter files.

``integral`` Is Zero or NaN
---------------------------

Possible causes:

* ``ellmin`` and ``ellmax`` fall outside the tabulated ``inputfile`` range;
* ``C_ell`` contains non-positive values and the log interpolation becomes
  invalid;
* the input table is not sorted by increasing ``ell``;
* the integration grid is too coarse for the angular scales.

Fix:
    Validate the input table and rerun a convergence check over ``ppp`` and
    ``ellmax``.

Build Warnings
--------------

The current C build can emit warnings related to unchecked ``system`` calls,
``sprintf`` format safety, and long logfile paths.  They do not block the
validated examples, but they should be addressed before hardening the package
for automated production pipelines.
