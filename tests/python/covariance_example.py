#  ============================================================================
#         1          2          3          4        ^ 5          6          7

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import AnalyticalCov as cov # Code to pass from C to Pyhton
import numpy as np
import matplotlib.pyplot as plt

ofilename = "analytic_covariance_22_noise.pdf"

thtdata = np.array([0.00232712, 0.00275673, 0.00326565, 0.00386852, 0.00458269,
       0.00542871, 0.00643091, 0.00761812, 0.00902451, 0.01069053,
       0.01266412, 0.01500206, 0.0177716 , 0.02105243, 0.02493894,
       0.02954293, 0.03499688, 0.04145769, 0.04911122, 0.05817769]) # Theta array

covariance = cov.compute_cov_noise(
    thtdata,
    rows=7, #rows, diagonals and dimension are in order to add a mask
    diagonals=4,
    dim=20,
    m=2, #multipole 1
    mp=2, #multipole 2
    noise=6.1e-11, #add noise
    output_file="analytic_covariance_22_noise.txt" #if you don't want to save the output just put None
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



