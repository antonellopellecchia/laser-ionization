# Overview

Library for simulating the response of particle detectors to laser beams.

The library calculates the number of primary electrons produced by a gaussian laser beam passing through the drift gap of a detectors. The interface exposed by the library is designed to be used similarly to the HEED library, used in high energy physics for the simulation of the energy loss of charged particles in gases.

# Theory

A laser beam passing through the drift gap of a gaseous particle detector typically frees electrons in the gas by two-photon ionization of low-potential impurity molecules naturally present in common counting gas mixtures. The number of electrons created by a uniform laser beam of flux per unit volume is

![](https://latex.codecogs.com/svg.latex?%5Cfrac%7BR%7D%7BV%7D%20%3D%20%5Cleft%28%5Cfrac%7B%5Clambda%7D%7Bhc%7D%5Cright%29%5E2%20I%5E2%20N%20%5Csigma%5E%7B%282%29%7D)

where I is the laser beam intensity and &sigma;<sup>(2)</sup> is the two-photon cross-section equivalent.

# Usage

# Examples