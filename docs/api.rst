API Reference
=============

This page summarizes the stable public interfaces in the current codebase.

Command-Line Interface
----------------------

Executable
    ``./wlcov``

Help
    ``./wlcov --help``

Parameter file
    ``./wlcov paramfile=path/to/parameters.txt``

See :doc:`user/command-line` for the full parameter table.

Python Module
-------------

.. py:module:: wlcovpy

.. py:class:: wlcov(default=True)

   Cython wrapper around the compiled ``wlcov`` core.

   :param bool default: If true, initialize wrapper defaults before user
      parameters are added.

   .. py:method:: set(*pars, **kwargs)

      Add or update runtime parameters.  Parameters may be supplied as one
      mapping positional argument or as keyword arguments.

      .. code-block:: python

         model.set({"inputfile": "tests/input/Cls_ep2.txt", "ellmax": 25})
         model.set(verbose=0, verbose_log=0)

   .. py:method:: Run(level=["MainLoop"])

      Execute the requested C routine chain.  The default runs input parsing,
      common initialization, parameter-file writing, thread setup, and
      ``MainLoop``.  Returns measured ``MainLoop`` CPU time.

   .. py:method:: clean()

      Clear the Python-side parameter dictionary.

   .. py:method:: clean_all()

      Release allocated C-side state and clear Python-side parameters.

   .. py:method:: getNThreads()

      Return the parsed thread count after ``Run`` has initialized the C
      structures.  The value is only operational when the package was compiled
      with OpenMP support.

Python Utility Functions
------------------------

.. py:module:: AnalyticalCov

``tests/python/AnalyticalCov.py`` is not installed as a package module, but it
provides reusable script-level functions.

.. py:function:: add_noise_to_column(input_file, output_file, noise)

   Copy a two-column table and add ``noise`` to the second column.

.. py:function:: calculate_integral(theta1, theta2, thetap1, thetap2, m, mp, ppp, inputfile, program="./IntegralCovMatrix")

   Run an external executable and parse ``integral=...`` or
   ``integrand(r=...)=...`` from stdout.

.. py:function:: build_mask(dim, rows, diagonals, symm=True)

   Build a boolean matrix mask used to select covariance entries.

.. py:function:: get_valid_indices(mask)

   Return selected matrix indices as ``(row, column)`` pairs.

.. py:function:: compute_cov_noise(thtdata, rows=6, diagonals=3, dim=20, m=0, mp=0, ppp=60, noise=0.0, input_file="./input/Cls_ep2.txt", temp_file="Cls_temp.txt", program="../wlcov", output_file=None)

   Compute a symmetric covariance matrix by calling the compiled core for each
   selected matrix element.

C-Level Entry Points
--------------------

The public C prototypes are declared in ``include/protodefs.h`` and related
headers:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Function
     - Purpose
   * - ``StartRun``
     - Initialize command-line/global structures from command-line arguments.
   * - ``StartRun_Common``
     - Shared initialization for command-line and Python-wrapper runs.
   * - ``MainLoop``
     - Execute the numerical integration workflow.
   * - ``PrintParameterFile``
     - Write the used-values parameter file.
   * - ``SetNumberThreads``
     - Configure OpenMP threads when OpenMP is enabled.
   * - ``EndRun``
     - Close logs and release allocated state.

The lower-level integration helpers in ``source/wlcov.c`` are currently
internal implementation details rather than a stable library API.
