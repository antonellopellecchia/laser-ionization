# Overview

Library for simulating the response of particle detectors to laser beams.

The library calculates the number of primary electrons produced by a gaussian laser beam passing through the drift gap of a detectors. The interface exposed by the library is designed to be used similarly to the HEED<sup>[1](#f1)</sup> library, used in high energy physics for the simulation of the energy loss of charged particles in gases. Its main application consists of coupling it with Garfield++<sup>[2](#f2)</sup> for simulation-aided laboratory tests on MPGDs.

# Theory

A laser beam passing through the drift gap of a gaseous particle detector typically frees electrons in the gas by two-photon ionization of low-potential impurity molecules naturally present in common counting gas mixtures<sup>[3](#f3)</sup>. The number of electrons created by a uniform laser beam of flux per unit volume is

![](https://latex.codecogs.com/svg.latex?%5Cfrac%7BR%7D%7BV%7D%20%3D%20%5Cleft%28%5Cfrac%7B%5Clambda%7D%7Bhc%7D%5Cright%29%5E2%20I%5E2%20N%20%5Csigma%5E%7B%282%29%7D)

where I is the laser beam intensity, N is the concentration of the impurity molecules and &sigma;<sup>(2)</sup> is the two-photon cross-section equivalent.

Using the explicit expression for the intensity of a gaussian beam<sup>[4](#f4)</sup>, the ionization density becomes

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
    Laser laser{laserWavelength, laserPulseEnergy, laserWaistRadius, laserWaistPosition};
    laser.SetGasVolume(point2, point2);
    ```
    
2. Call `Pulse()` to calculate the number of primary electrons. This is done by integrating the ionization density function, creating a Poisson distribution with average equal to the result of the integration and sampling from such Poisson distribution.
    ``` int nprimaries = laser.Pulse(); ```

3. Call `GetPrimaryElectron()` to obtain the position of each electron created by the pulse. This is obtained by performing a rejection sampling from the ionization density distribution:
    ```
      Point3D electronPosition;
      for (int i=0; i<nprimaries; i++) {
        electronPosition = laser->GetPrimaryElectron();
        // ...
        // do stuff with the position, e.g. track electron drift or avalanche
        // ...
      }
    ```
    
# Examples

- The `basic` example is meant for test in a minimal environment and has no particular dependencies; it calls the `Pulse()` function many times and prints the Poisson-distributed number of electrons created by each pulse.

- The `gem` example couples the library with Garfield++ to simulate the avalanche of electrons created by the laser beam. Its dependencies are Garfield++ and ROOT. In the `build/plots` directory one can find output plots, such as the electron drift lines and the distribution of the primary electrons as calculated witht the methods described above. As expected, the position of the primary electrons follows a gaussian distribution on the `y-z` directions.

The examples can be built with `cmake` by moving to the corresponding directory and using:

    mkdir build && cd build
    cmake ..
    make

# References

<a name="f1">1</a>. I.B. Smirnov, [Modeling of ionization produced by fast charged particles in gases](https://doi.org/10.1016/j.nima.2005.08.064)

<a name="f2">2</a>. R. Veenhof, [Numerical methods in the simulation of gas-based detectors](https://doi.org/10.1088/1748-0221/4/12/P12017)

<a name="f3">3</a>. H. J. Hilke, [Detector calibration with lasers - a review](https://doi.org/10.1016/0168-9002(86)91177-0)

<a name="f4">4</a>. B. Saleh and M. C. Teich, [Fundamentals of photonics](https://doi.org/10.1002/0471213748)
