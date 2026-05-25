Outputs and File Formats
========================

The current ``wlcov`` C core writes numerical results to standard output and
provenance files under ``rootDir``.  Downstream scripts should treat the stdout
format as a lightweight interface and record the used-values file for
reproducibility.

Standard Output
---------------

A compact run prints:

* the input angular parameters in radians and arcminutes;
* ``ellmin``, ``ellmax``, ``inputfile``, ``m``, ``mp``, and ``ppp``;
* intermediate terms such as ``xi`` and ``F``-type integrals;
* ``integrand(r=...)=...`` for the selected radius;
* ``integral=...`` for the final radial integration;
* CPU timings for major blocks.

Example parser:

.. code-block:: python

   import re
   import subprocess

   result = subprocess.run(
       [
           "./wlcov",
           "inputfile=tests/input/Cls_ep2.txt",
           "ellmax=25",
           "ppp=4",
           "verbose=0",
           "verbose_log=0",
       ],
       check=True,
       capture_output=True,
       text=True,
   )

   match = re.search(r"integral=\\s*([+-]?\\d*\\.?\\d+(?:[eE][+-]?\\d+)?)", result.stdout)
   integral = float(match.group(1))

Provenance Files
----------------

Every run with a non-empty ``rootDir`` writes a parameter record:

.. code-block:: text

   rootDir/parameters_null-wlcov-usedvalues

For parameter-file runs, the basename changes to match the input parameter file
name:

.. code-block:: text

   rootDir/<parameter-file-name>-usedvalues

Logs
----

When ``verbose_log`` is positive, ``wlcov`` also writes:

.. code-block:: text

   rootDir/tmp/wlcov.log

Use ``verbose=0 verbose_log=0`` for automated test runs where only stdout and
used-values files are needed.

Python Utility Outputs
----------------------

``tests/python/covariance_example.py`` demonstrates a small covariance-matrix
workflow.  It writes:

.. code-block:: text

   tests/analytic_covariance_quickstart.txt
   tests/analytic_covariance_quickstart.pdf

The text file is a whitespace-delimited numeric matrix written by
``numpy.savetxt``.  The PDF is a diagnostic image of ``log10(abs(covariance))``.

Recommended Production Output Practice
--------------------------------------

For publication workflows, store generated stdout, used-values files, input
tables, plotting scripts, and environment metadata together.  If the stdout
format becomes a stable public interface, prefer adding a structured output
option such as CSV, HDF5, or FITS rather than parsing free-form console output.
