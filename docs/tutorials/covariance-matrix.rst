Small Covariance-Matrix Workflow
================================

``tests/python/AnalyticalCov.py`` demonstrates how to wrap repeated executable
calls in a NumPy workflow.  The bundled example is intentionally small so it can
serve as a regression smoke test.

Run from the ``tests`` directory:

.. prompt:: bash

   cd tests
   python3 python/covariance_example.py

The script:

* creates a noisy temporary copy of ``input/Cls_ep2.txt``;
* builds a compact mask for a four-element angular grid;
* calls ``../wlcov`` for each selected matrix element;
* writes ``analytic_covariance_quickstart.txt``;
* writes ``analytic_covariance_quickstart.pdf``.

For larger matrices, make the temporary ``C_ell`` filename and output directory
unique for each process before parallelizing the workflow.
