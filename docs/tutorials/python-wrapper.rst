Python Wrapper Workflow
=======================

This tutorial runs the Cython wrapper and verifies that the extension can
import, configure a model, run the C core, and clean up.

1. Build and install the wrapper:

   .. prompt:: bash

      make PYTHON=python3 GSL_INCL="-I/usr/include" \
           GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all

2. Run the documented example:

   .. prompt:: bash

      python3 docs/examples/python_wrapper.py

3. Inspect the used-values file:

   .. prompt:: bash

      cat Output_python_example/wlcovpy_param.txt-usedvalues

The example uses absolute paths derived from its file location, so it can be
run from any working directory after the wrapper is installed.

The current wrapper returns CPU time rather than the numerical covariance
integral.  To capture the printed integral from Python, run the executable with
``subprocess`` as shown in :doc:`../user/outputs`.
