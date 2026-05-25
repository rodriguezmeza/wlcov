#!/usr/bin/env python3
"""Run a compact wlcov calculation through the Python wrapper."""

from pathlib import Path

from wlcovpy import wlcov


ROOT = Path(__file__).resolve().parents[2]

model = wlcov(default=False)
model.set(
    {
        "inputfile": str(ROOT / "tests" / "input" / "Cls_ep2.txt"),
        "rootDir": str(ROOT / "Output_python_example"),
        "r": 0.01,
        "theta1": 0.01,
        "theta2": 0.012,
        "thetap1": 0.011,
        "thetap2": 0.013,
        "m": 0,
        "mp": 0,
        "ppp": 4,
        "ellmin": 1.0,
        "ellmax": 25.0,
        "numberThreads": 1,
        "verbose": 0,
        "verbose_log": 0,
        "options": "",
    }
)

try:
    cputime = model.Run()
finally:
    model.clean_all()

print(f"MainLoop CPU time: {cputime:.6g} s")
