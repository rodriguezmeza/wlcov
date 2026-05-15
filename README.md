# wlcov

Code to compute the Gaussian piece of the 3PCF in a harmonic basis of scalar fields over the sphere within Limber approximation

## Authors

- **Sofía Samario-Nava**  
  ssamario@icf.unam.mx

- **Alejandro Avilés**  
  aviles@icf.unam.mx

Contributor:

- [Mario A. Rodriguez-Meza(https://github.com/rodriguezmeza)
  marioalberto.rodriguezmeza@gmail.com

## Compilation

This code uses the **GNU Scientific Library (GSL)** for the numerical integration routines and the evaluation of Bessel functions.

Make sure GSL is installed on your system before compiling.

To compile the code for the first time, run:
```
gcc -Wall -I/path/to/gsl/include -c IntegralCovMatrix.c
gcc -Wall IntegralCovMatrix.o -L/path/to/gsl/lib -lgsl -lgslcblas -lm -o IntegralCovMatrix
```

Replace ```/path/to/gsl/``` with the actual installation path of GSL on your system.

### Running the Code

After compilation, execute:
```
./IntegralCovMatrix
```
## Python usage

To run the code in Python, import the `AnalyticalCov.py` module, which contains the functions needed to compute the covariance matrix.

An example of how to use these functions is provided in `Covariance_example.ipynb`.

