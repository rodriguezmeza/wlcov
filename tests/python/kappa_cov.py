#!/usr/bin/env python3
"""Minimal Python-wrapper example for wlcov.

This script intentionally uses the same reduced parameter set exposed by the
current C core.  It is meant as a fast smoke test and as a copyable example for
new users.
"""

import argparse
from pathlib import Path

from wlcovpy import wlcov


def run_wlcov(
    inputfile,
    outdir,
    ellmin=1.0,
    ellmax=25.0,
    ppp=4,
    number_threads=1,
):
    """Run a small wlcov calculation through the Cython wrapper."""
    model = wlcov(default=False)
    model.set(
        {
            "inputfile": str(inputfile),
            "rootDir": str(outdir),
            "r": 0.01,
            "theta1": 0.01,
            "theta2": 0.012,
            "thetap1": 0.011,
            "thetap2": 0.013,
            "m": 0,
            "mp": 0,
            "ppp": ppp,
            "ellmin": ellmin,
            "ellmax": ellmax,
            "numberThreads": number_threads,
            "verbose": 0,
            "verbose_log": 0,
            "options": "",
        }
    )
    try:
        return model.Run()
    finally:
        model.clean_all()


def main():
    parser = argparse.ArgumentParser(
        description="Run a compact wlcov Python-wrapper smoke test."
    )
    parser.add_argument(
        "--fnamePS",
        type=Path,
        default=Path("input/Cls_ep2.txt"),
        help="Two-column ell, C_ell input file.",
    )
    parser.add_argument(
        "--outdir",
        type=Path,
        default=Path("Output_python"),
        help="Output directory for used-parameter files.",
    )
    parser.add_argument("--ellmin", type=float, default=1.0)
    parser.add_argument("--ellmax", type=float, default=25.0)
    parser.add_argument("--ppp", type=int, default=4)
    parser.add_argument("--threads", type=int, default=1)
    args = parser.parse_args()

    cputime = run_wlcov(
        args.fnamePS,
        args.outdir,
        ellmin=args.ellmin,
        ellmax=args.ellmax,
        ppp=args.ppp,
        number_threads=args.threads,
    )
    print(f"MainLoop CPU time: {cputime:.6g} s")


if __name__ == "__main__":
    main()
