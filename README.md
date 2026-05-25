# wlcov

`wlcov` computes the Gaussian contribution to weak-lensing three-point
covariance terms in a harmonic basis on the sphere under the Limber
approximation.  The project provides a compiled C executable, a static library,
a Cython wrapper (`wlcovpy`), and small Python examples for scripted workflows.

## Documentation

The full Sphinx documentation lives in `docs/` and is structured for
ReadTheDocs-style publication:

```bash
python3 -m pip install --user -r docs/requirements.txt
cd docs
make html
make man
make latexpdf
```

The generated artifacts are `docs/_build/html/index.html`,
`docs/_build/man/wlcov.1`, and `docs/_build/latex/wlcov.pdf`.

## Quick Install

Install Python build dependencies, then build the C executable and wrapper:

```bash
python3 -m pip install --user numpy Cython
make clean
make PYTHON=python3 GSL_INCL="-I/usr/include" \
     GSL_LIBS="-L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas" all
```

For a custom GSL installation, set `GSL_DIR`, `GSL_INCLUDE_DIR`, and
`GSL_LIBRARY_DIR`, and pass matching `GSL_INCL` and `GSL_LIBS` values to
`make`.

## Quick Run

```bash
./wlcov inputfile=tests/input/Cls_ep2.txt rootDir=Output_quick \
   r=0.01 theta1=0.01 theta2=0.012 thetap1=0.011 thetap2=0.013 \
   m=0 mp=0 ellmin=1 ellmax=25 ppp=4 verbose=0 verbose_log=0
```

Python wrapper smoke test:

```bash
cd tests
python3 python/kappa_cov.py --fnamePS input/Cls_ep2.txt \
   --outdir Output_python --ellmax 25 --ppp 4
```

## Authors

- Sofia Samario-Nava, ssamario@icf.unam.mx
- Alejandro Aviles, aviles@icf.unam.mx

Contributor:

- Mario A. Rodriguez-Meza, marioalberto.rodriguezmeza@gmail.com

## License

`wlcov` is distributed under the MIT license.  If you use this program in
research that results in publications, please cite Sofia Samario et al.,
arXiv:2506.19811, and include the code version and runtime configuration used
for the analysis.
