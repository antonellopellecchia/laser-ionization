# Overview

Library for simulating the response of particle detectors to laser beams.

The library calculates the number of primary electrons produced by a gaussian laser beam passing through the drift gap of a detectors. The interface exposed by the library is designed to be used similarly to the HEED library, used in high energy physics for the simulation of the energy loss of charged particles in gases.

# Theory

A laser beam passing through the drift gap of a gaseous particle detector typically frees electrons in the gas by two-photon ionization of low-potential impurity molecules naturally present in common counting gas mixtures. The number of electrons created by a uniform laser beam of flux per unit volume is

<p align=center>
  ![](https://latex.codecogs.com/svg.latex?%5Cfrac%7BR%7D%7BV%7D%20%3D%20%5Cleft%28%5Cfrac%7B%5Clambda%7D%7Bhc%7D%5Cright%29%5E2%20I%5E2%20N%20%5Csigma%5E%7B%282%29%7D)
</p>

where I is the laser beam intensity, N is the concentration of the impurity molecules and &sigma;<sup>(2)</sup> is the two-photon cross-section equivalent.

Using the explicit expression for the intensity of a gaussian beam, the ionization density becomes

<p align=center>
  ![](https://latex.codecogs.com/svg.latex?%5Cfrac%7BdR%7D%7Bdxdydz%7D%20%3D%20%5Cfrac%7BN%5Csigma%5E%7B%282%29%7D%7D%7B%28h%5Cnu%5E2%29%7D%20%5Cleft%5B%5Cfrac%7B2P%7D%7B%5Cpi%20w_0%5E2%7D%5Cleft%28%5Cfrac%7Bw_0%7D%7Bw%28z%29%5E2%7D%20%5Cright%29%5E2%20%5Ctext%7Be%7D%5E%7B-2%28x%5E2&plus;y%5E2%29/w%28z%29%5E2%7D%5Cright%5D%5E2)
</p>

Sampling from this distribution gives the position of each electron created, while integrating it over the gas volume the total number of electrons created can be obtained. The present library performs these two calculations, assuming the cross-section N&sigma;<sup>(2)</sup> is known (it should be obtained by laboratory measurements).

# Usage

# Examples
