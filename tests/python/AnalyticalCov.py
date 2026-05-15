import numpy as np
import subprocess
import re
from typing import Optional


# ----------------------------
# File utilities
# ----------------------------
def add_noise_to_column(input_file: str, output_file: str, noise: float):
    with open(input_file) as infile, open(output_file, "w") as outfile:
        for line in infile:
            if not line.strip():
                outfile.write("\n")
                continue

            parts = line.split()
            if len(parts) != 2:
                outfile.write(line)
                continue

            try:
                val = float(parts[1]) + noise
                outfile.write(f"{parts[0]}\t{val}\n")
            except ValueError:
                outfile.write(line)


# ----------------------------
# External program wrapper
# ----------------------------
def _run_integral(args, verbose=False):
    try:
        result = subprocess.run(args, check=True, capture_output=True, text=True)

        if verbose:
            print(result.stdout)

        patterns = [
            r'integral=\s*([+-]?\d*\.?\d+(?:[eE][+-]?\d+)?)',
            r'integrand\(r=[\d.eE+-]+\)=\s*([+-]?\d*\.?\d+(?:[eE][+-]?\d+)?)'
        ]

        for pattern in patterns:
            match = re.search(pattern, result.stdout)
            if match:
                return float(match.group(1))

        # If nothing matched
        print("No valid value found in output:")
        print(result.stdout)
        return None

    except subprocess.CalledProcessError as e:
        print(e.stderr)
        return None


def calculate_integral(theta1, theta2, thetap1, thetap2,
                       m, mp, ppp, inputfile, program="./IntegralCovMatrix"):

    args = [
        program,
        f"theta1={theta1}",
        f"theta2={theta2}",
        f"thetap1={thetap1}",
        f"thetap2={thetap2}",
        f"m={m}",
        f"mp={mp}",
        f"ppp={ppp}",
        f"inputfile={inputfile}"
    ]

    return _run_integral(args)


# ----------------------------
# Mask builder (vectorized)
# ----------------------------
def build_mask(dim, rows, diagonals, symm=True):
    mask = np.ones((dim, dim), dtype=bool)

    mask[:rows, :] = False
    mask[:, :rows] = False

    for d in range(diagonals):
        mask[np.abs(np.subtract.outer(np.arange(dim), np.arange(dim))) == d] = False

    if symm:
        mask = np.triu(mask)

    return mask


# ----------------------------
# Index extraction
# ----------------------------
def get_valid_indices(mask):
    rows, cols = np.where(mask)
    return np.column_stack((rows, cols))


# ----------------------------
# Core computation
# ----------------------------
def compute_cov_noise(
    thtdata,
    rows=6,
    diagonals=3,
    dim=20,
    m=0,
    mp=0,
    ppp=60,
    noise=0.0,
    input_file="./input/Cls_ep2.txt",
    temp_file="Cls_temp.txt",
    program="../wlcov",
    output_file=None ):
    """
    Main user-facing function.
    """

    # Step 1: prepare noisy file ONCE
    add_noise_to_column(input_file, temp_file, noise)

    # Step 2: build mask + indices
    mask = build_mask(dim, rows, diagonals)
    indices = get_valid_indices(mask)

    n = len(indices)
    cov = np.zeros((n, n))

    # Step 3: compute matrix
    for i in range(n):
        if i % 5 == 0:
            print(f"Progress: {i}/{n}")

        for j in range(i + 1):  # exploit symmetry
            idx1 = indices[i]
            idx2 = indices[j]

            th1, th2 = thtdata[idx1]
            th1p, th2p = thtdata[idx2]

            val = calculate_integral(
                th1, th2, th1p, th2p,
                m, mp, ppp,
                temp_file,
                program)

            cov[i, j] = val
            cov[j, i] = val
            
    if output_file is not None:
        np.savetxt(output_file, cov, delimiter=" ")
        print(f"Saving covariance matrix to: {output_file}")

    return cov
