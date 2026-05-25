#  ============================================================================
#         1          2          3          4        ^ 5          6          7

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import AnalyticalCov as cov # Code to pass from C to Python
import numpy as np
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

ofilename = "analytic_covariance_quickstart.pdf"

thtdata = np.array([0.00232712, 0.00275673, 0.00326565, 0.00386852])

covariance = cov.compute_cov_noise(
    thtdata,
    rows=1,
    diagonals=1,
    dim=4,
    m=2, #multipole 1
    mp=2, #multipole 2
    ppp=4,
    noise=6.1e-11, #add noise
    input_file="./input/Cls_ep2.txt",
    output_file="analytic_covariance_quickstart.txt" #if you don't want to save the output just put None
)

plt.figure(figsize=(6,5))
plt.imshow(np.log10(abs(covariance)))
plt.colorbar(label="Covariance")
plt.title("Covariance Matrix")
plt.xlabel("Index")
plt.ylabel("Index")
plt.savefig(ofilename,dpi=300)
# if you want plot to be shown, uncomment this line
#plt.show()

