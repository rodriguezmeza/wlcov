Command-Line Usage
==================

The executable accepts command-line options as ``name=value`` tokens.  Do not
include spaces around ``=`` on the command line.

.. prompt:: bash

   ./wlcov inputfile=tests/input/Cls_ep2.txt ellmax=25 ppp=4

Use ``--help`` to print defaults:

.. prompt:: bash

   ./wlcov --help

Syntax
------

``wlcov`` can be driven entirely from command-line parameters:

.. code-block:: text

   ./wlcov name=value name=value ...

or from a parameter file:

.. code-block:: text

   ./wlcov paramfile=path/to/parameters.txt

Parameter files use the form ``name = value`` and may include comments starting
with ``#`` or ``%``.  Short aliases are accepted on the command line, but use
full parameter names in parameter files.

Parameters
----------

.. list-table::
   :header-rows: 1
   :widths: 20 16 16 48

   * - Name
     - Alias
     - Default
     - Description
   * - ``paramfile``
     -
     -
     - Parameter file to read before running.
   * - ``r``
     -
     - ``0.116355``
     - Radial separation in radians.
   * - ``theta1``
     -
     - ``0.1``
     - First angular scale in radians.
   * - ``theta2``
     -
     - ``0.1``
     - Second angular scale in radians.
   * - ``thetap1``
     -
     - ``0.1``
     - First primed angular scale in radians.
   * - ``thetap2``
     -
     - ``0.1``
     - Second primed angular scale in radians.
   * - ``m``
     -
     - ``0``
     - Harmonic multipole index.
   * - ``mp``
     -
     - ``0``
     - Primed harmonic multipole index.
   * - ``ppp``
     -
     - ``20``
     - Integration sampling density, in points per Bessel period.
   * - ``inputfile``
     - ``in``
     - ``CkappaT_ep.dat``
     - Two-column ``ell`` and ``C_ell`` input table.
   * - ``rootDir``
     - ``root``
     - ``Output``
     - Output directory for provenance files and logs.
   * - ``ellmin``
     -
     - ``1``
     - Lower multipole limit.
   * - ``ellmax``
     -
     - ``2000.0``
     - Upper multipole limit.
   * - ``verbose``
     - ``verb``
     - ``2``
     - Console verbosity.  Use ``0`` for quiet scripted runs.
   * - ``verbose_log``
     - ``verblog``
     - ``1``
     - Log-file verbosity.  When positive, writes ``rootDir/tmp/wlcov.log``.
   * - ``numberThreads``
     - ``nthreads``
     - ``16``
     - OpenMP thread count when compiled with ``OPENMPMACHINE=1``.
   * - ``options``
     - ``opt``
     -
     - Space- or comma-separated runtime flags.  ``make-info`` prints build settings.

Example
-------

.. prompt:: bash

   ./wlcov inputfile=tests/input/Cls_ep2.txt rootDir=Output_cli \
      r=0.01 theta1=0.01 theta2=0.012 thetap1=0.011 thetap2=0.013 \
      m=0 mp=0 ellmin=1 ellmax=25 ppp=4 verbose=0 verbose_log=0

The main numerical result is emitted as lines containing ``integrand(...)`` and
``integral=...``.  See :doc:`outputs` for output parsing notes.
