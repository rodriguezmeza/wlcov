Build Configuration
===================

``wlcov`` uses a Makefile-based build.  User-facing settings live in
``Makefile_settings`` and machine-specific compiler/linker settings live in
``Makefile_machine``.

Makefile Settings
-----------------

.. list-table::
   :header-rows: 1
   :widths: 24 16 60

   * - Setting
     - Default
     - Meaning
   * - ``USEGSL``
     - ``1``
     - Enables GSL-backed Bessel-function routines.  Keep enabled for current code.
   * - ``GSLINTERNAL``
     - ``0``
     - Reserved for bundled GSL sources.  The validated path uses system GSL.
   * - ``OPENMPMACHINE``
     - ``0``
     - Enables OpenMP runtime thread control when set to ``1``.
   * - ``ADDONSON``
     - ``1``
     - Enables the CLASS-style input parser, getparam support, and Cython support.

Compiler and Linker Variables
-----------------------------

The main variables in ``Makefile_machine`` are:

.. list-table::
   :header-rows: 1
   :widths: 24 56

   * - Variable
     - Purpose
   * - ``CC``
     - C compiler.  The default is ``gcc``.
   * - ``PYTHON``
     - Python executable used to install ``wlcovpy``.  Override with ``PYTHON=python3`` if ``python`` is unavailable.
   * - ``OPTFLAG``
     - Optimization flags, currently ``-O3 -ffast-math``.
   * - ``OMPFLAG``
     - OpenMP compiler flag, currently ``-fopenmp``.
   * - ``GSL_INCL``
     - Include path for GSL headers.
   * - ``GSL_LIBS``
     - Linker search path and GSL libraries.

Recommended Build Pattern
-------------------------

Prefer command-line overrides for machine-local paths so repository defaults
remain portable:

.. prompt:: bash

   make clean
   make PYTHON=python3 \
        GSL_INCL="-I/usr/include" \
        GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all

For a local GSL installation:

.. prompt:: bash

   export GSL_DIR="$HOME/local/gsl"
   export GSL_INCLUDE_DIR="$GSL_DIR/include"
   export GSL_LIBRARY_DIR="$GSL_DIR/lib"
   make PYTHON=python3 \
        GSL_INCL="-I${GSL_INCLUDE_DIR}" \
        GSL_LIBS="-L${GSL_LIBRARY_DIR} -lgsl -lgslcblas" all

Build Artifacts
---------------

``make all`` builds the executable and static library, installs ``wlcovpy`` via
``pip install .``, and copies ``wlcov`` to
``tests/notebooks_r2d2/IntegralCovMatrix`` for the legacy notebook workflow.

For a clean release tree, run:

.. prompt:: bash

   make clean

and remove generated example outputs before committing.
