# Overview

Library for simulating the response of particle detectors to laser beams.

The library calculates the number of primary electrons produced by a gaussian laser beam passing through the drift gap of a detectors. The interface exposed by the library is designed to be used similarly to the HEED library, used in high energy physics for the simulation of the energy loss of charged particles in gases. Its main application consists of coupling it with Garfield++ for simulation-aided laboratory tests on MPGDs.

# Theory

A laser beam passing through the drift gap of a gaseous particle detector typically frees electrons in the gas by two-photon ionization of low-potential impurity molecules naturally present in common counting gas mixtures. The number of electrons created by a uniform laser beam of flux per unit volume is

![](https://latex.codecogs.com/svg.latex?%5Cfrac%7BR%7D%7BV%7D%20%3D%20%5Cleft%28%5Cfrac%7B%5Clambda%7D%7Bhc%7D%5Cright%29%5E2%20I%5E2%20N%20%5Csigma%5E%7B%282%29%7D)

where I is the laser beam intensity, N is the concentration of the impurity molecules and &sigma;<sup>(2)</sup> is the two-photon cross-section equivalent.

Using the explicit expression for the intensity of a gaussian beam, the ionization density becomes

![](https://latex.codecogs.com/svg.latex?%5Cfrac%7BdR%7D%7Bdxdydz%7D%20%3D%20%5Cfrac%7BN%5Csigma%5E%7B%282%29%7D%7D%7B%28h%5Cnu%5E2%29%7D%20%5Cleft%5B%5Cfrac%7B2P%7D%7B%5Cpi%20w_0%5E2%7D%5Cleft%28%5Cfrac%7Bw_0%7D%7Bw%28z%29%5E2%7D%20%5Cright%29%5E2%20%5Ctext%7Be%7D%5E%7B-2%28x%5E2&plus;y%5E2%29/w%28z%29%5E2%7D%5Cright%5D%5E2)

Sampling from this distribution gives the position of each electron created, while integrating it over the gas volume returns the average number of electrons created by a single laser pulse. The present library performs these two calculations, assuming the cross-section N&sigma;<sup>(2)</sup> is known (it should be obtained by laboratory measurements).

# Environment setup

1. Clone the repository

    ``` git clone https://github.com/antonellopellecchia/laser-ionization.git ```

2. Set the environment variable LASER_HOME, pointing to the directory where the repository was cloned, e.g.

    ``` export LASER_HOME="$HOME/Code/laser-ionization" ```

# Usage

1. Create a `Laser` object (passing to it the wavelength and waist size) and set volume boundaries, beam waist position and pulse energy:

    ```
    Laser *laser = new Laser(laserWavelength, laserWaistRadius);
    laser->SetWaistPosition(0., 0., 0.);
    laser->SetGasVolume(-0.5*sizeX, -0.5*sizeY, -0.5*sizeZ, +0.5*sizeX, +0.5*sizeY, +0.5*sizeZ);
    laser->SetPulseEnergy(51e-8);
    laser->SetDebugging(true);
    laser->Initialize();
    ```
    
2. Call `Pulse()` to calculate the number of primary electrons. This is done by integrating the ionization density function, creating a Poisson distribution with average equal to the result of the integration and sampling from such Poisson distribution.
    ``` int nprimaries = laser->Pulse(); ```

3. Call `GetPrimaryElectron()` to obtain the position of each electron created by the pulse. This is obtained by performing a rejection sampling from the ionization density distribution:
    ```
      double x0, y0, z0;
      for (int i=0; i<nprimaries; i++) {
        laser->GetPrimaryElectron(x0, y0, z0);
        // ...
        // do stuff with the position, e.g. track electron drift or avalanche
        // ...
      }
    ```
    
# Examples
