# wlcov

Code to compute the Gaussian piece of the 3PCF in a harmonic basis of scalar fields over the sphere within Limber approximation

## Authors

- **Sofía Samario-Nava**  
  ssamario@icf.unam.mx

- **Alejandro Avilés**  
  aviles@icf.unam.mx

Contributor:

- [Mario A. Rodriguez-Meza(https://github.com/gnizq64)
  marioalberto.rodriguezmeza@gmail.com

## Table of Contents

-   [Introduction](#introduction)
-   [Compiling and getting started](#compiling-and-getting-started)
-   [Configuration](#configuration)
-   [Parameters](#parameters)
-   [Python](#python)
-   [License](#license)
-   [Acknowledgements](#acknowledgements)

## Introduction

**wlcov** is a C code for computing correlation functions using SPT, EFT and Halo model. So far can compute only the 3-point correlation function (3pcf) for galaxy weak leansing convergence in a plane wave expansion.

A short documentation can be found in a html version of the man (linux manual) version man. Look for it (`docs/man/wlcf.html`) an open it with a web explorer.

## Compiling and getting started

Download the code by cloning it from https://github.com/rodriguezmeza/wlcov.

Dependencies: wlcf needs gsl installed in your system. Sources are included. If your system does not have it, try installing them. In case of problems go to web page https://www.gnu.org/software/gsl/ for details or ask to your system administrator. Make necessary changes in `Makefile_machine` file and look up for `GSL` at the end of the file.

Go to the wlcov directory (`cd wlcov/`) and compile (`make clean; make all`). If the first compilation attempt fails, you may need to open the Makefile_machine file and adapt the name of the compiler (default: gcc), of the optimization flag (default: `-O4 -ffast-math`) and of the OpenMP flag (default: `-fopenmp`; this flag is optional, you are free to compile without OpenMP if you don't want parallel execution; note that you will need the version 4.2 or higher of gcc to be able to compile with `-fopenmp`). The code has been tested with gcc version 10 and 12 and would be working with version 11, and 13. (In particular, for compiling on Mac >= 10.9 despite of the clang incompatibility with OpenMP).

To check that the code runs, if you are in `wlcov` directory, type:

    $ make clean; make all
    $ cd tests
    $ ../wlcov

It will run using all default values and a directory named `Output` will be created under `tests`. **wlcov** will save all output files and a log file in `Output/tmp`. A file with the parameter values use in the run named `parameters_null-usedvalues` will also be saved. You may use it as a template to create your own parameter files.

You may consult the code´s man page for more detailed information on how to run **wlcov**:

    $ man ../docs/man/wlcov.1


## Configuration

**wlcov** can be configured by switching on/off some options. Configuration file is `Makefile_setting`.

| Option         | Description                                                                                                                                                   |
|:--------------:|---------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `USEGSL`       | `= 1` switch on/off computation using GSL routines. Always set it to `1`.                                                                                     |
| `OPENMPMACHINE`     | `= 1` for enabling OpenMP parallelism<br />(Specify the corresponding compiler flag in `Makefile_machine` file)                                                             |
| `ADDONSON`  | `= 1` for adding more funcionality to the code, like other searching methods, other catalog formats                                                                                                |

**Note**:
After changing `Makefile_settings` in order to have the new settings active in **wlcov** you have to re-compile the code: `make clean; make all`. 

## Parameters

The list of available command line parameters can be consulted using the `-h` or `--help` flags:

    $ ../wlcov --help

See also the man page as explained above. If you execute:

    $ ../wlcov --clue

you will receive in response how **wlcov** should be executed using command line parameters. Just pick up the parameteres you need and, if necessary, modify their values according to your needs.

## Python

To run the code in Python, import the `AnalyticalCov.py` module, which contains the functions needed to compute the covariance matrix and execute:

    $ python ./python/covariance_example.py

To install wlcf python module (wlcovpy) just execute (you already do it...):

    $ make clean; make all

To test it go to directory `tests` and run:

    $ python ./python/kappa_cov.py

Note: this interface in Cython was tested in a python environment with `python3.12`.

## Plotting utilities

Several Jupyter notebooks, written by `xxx`, are available to process wlcov results. They are in the folder `tests/notebooks`. 

Other python scripts are in directory `tests/python`.


## License

**wlcov** was written by Sofia Samario & A. Aviles et al., is open source and distributed under the [MIT license](LICENSE). If you use this program in research work that results in publications, please cite the following paper:

Sofia Samario et al., [arXiv:2506.19811](https://arxiv.org/abs/2506.19811)

## Acknowledgements

wlcov use/is based on the following codes or projects:
-   [Zeno](https://home.ifa.hawaii.edu/users/barnes/zeno/index.html)
-   [Numerical recipies](https://numerical.recipes/)
-   [GSL](https://www.gnu.org/software/gsl/)
-   [CLASS](https://github.com/lesgourg/class_public)

This work was supported by UNAM PAPIIT grant IA101825 and SECIHTI grant CBF2023-2024-162.

