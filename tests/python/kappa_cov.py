#  ============================================================================
#         1          2          3          4        ^ 5          6          7

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
import os, argparse, gc, numpy as np
from pathlib import Path
import healpy as hp
#B for cBalls
import sys
# Determine the absolute path of the target (wlcfpy) directory
#   these two lines won´t be necessary if wlcfpy is in searching path
#target_directory = os.path.abspath('/opt/homebrew/anaconda3/lib/python3.13/site-packages/')
# Append the directory to sys.path
#sys.path.append(target_directory)
from wlcfpy import wlcf
#E

# -------- wlcov --------
def wlcov(ellmin, ellmax, chiQuadSteps, Nell, zbin, mMax,
                nthreads=16, outdir=None, psfile=None, tree_level=3):
    Bells = wlcf()
    Bells.set({'z':1.0334})
    Bells.set({'h':0.7})
    Bells.set({'sigma8':0.82})
    Bells.set({'Omb':0.046})
    Bells.set({'Omc':0.233})
    Bells.set({'ns':0.97})
    Bells.set({'w':-1.0})
    Bells.set({'Omnu':0.0})
    Bells.set({'fnamePS':psfile})
    Bells.set({'prefix':'run1_'})
    if outdir == None:
        Bells.set({'path_Bells':'Bell_outputs'})
    else:
        Bells.set({'path_Bells':outdir})
    Bells.set({'tree_level':tree_level})
    Bells.set({'zbin':zbin})
    Bells.set({'mMax':mMax})
    Bells.set({'chiQuadSteps':chiQuadSteps})
    Bells.set({'GLpoints':64})
    Bells.set({'Nell':Nell})
    Bells.set({'ellmax':ellmax})
    Bells.set({'ellmin':ellmin})
    Bells.set({'Wg':0})
    Bells.set({'fWgchi':"./input/Wg_Takahashi_z05078.txt"})
    Bells.set({'writevectors':1})
    Bells.set({'numberThreads':nthreads})
    Bells.set({'rootDir':outdir})
    Bells.set({'options':''})
    Bells.set({'chatty':2})
    Bells.set({'verbose':1})
    Bells.set({'verbose_log':0})

    print('Running...')
    cputime=Bells.Run()

#    nmonopoles = Balls.getnMonopoles()
    
    # getHistZetaM_sincos(m, type): m multipole,
    #   type: 1 - cos; 2 - sin; 3 - sincos; 4 - cossin
    # edge_effects not programmed yet
    monopolesData = []
#    for j in range(1,nmonopoles+1):
#        zcos = Balls.getHistZetaMsincos(j, 1)
#        zsin = Balls.getHistZetaMsincos(j, 2)
#        monopolesData.append(zcos+zsin)
    print('MainLoop cputime=',cputime,' sec.')
    print('cleaning all...')
    Bells.clean_all()
    print('done.')
    gc.collect()
    return monopolesData

def main():
    ap = argparse.ArgumentParser(description="Gaussian piece of the 3PCF modeling with wlcov.")
    ap.add_argument("--fnamePS", type=Path, required=True,
                    help="Path to the linear power spectrum.")
    ap.add_argument("--outdir", default="Bell_outputs",
                    help="Output directory (default: 'Bell_output').")
    ap.add_argument("--threads", type=int, default=max(1, (os.cpu_count() or 2)-1))
    ap.add_argument("--zbin", type=float, default=0.5078,
                    help="z bins.")
    ap.add_argument("--mMax", type=int, default=5,
                    help="Bm moments up to mMax.")
    ap.add_argument("--chiQuadSteps", type=int, default=300)
    ap.add_argument("--Nell", type=int, default=128)
    ap.add_argument("--ellmax", type=float, default=2000.0)
    ap.add_argument("--ellmin", type=float, default=1)
    ap.add_argument("--tree_level", type=int, default=3)
    args = ap.parse_args()

    # Computing convergence 2pcf/3pcf
    zeta = wlcf_Bells(args.ellmin, args.ellmax,
            args.chiQuadSteps, args.Nell,
            args.zbin, args.mMax, nthreads=args.threads, outdir=args.outdir,
            psfile=args.fnamePS, tree_level=args.tree_level)
    print()
    print()
    print('zeta monopole:')
#    print(zeta[0])

if __name__ == "__main__":
    main()
