# Class description

## `Laser`

Class to determine the number and position of primary electrons given specific laser parameters (pulse energy, waist radius, wavelength etc.).

- `Laser(double wavelength, double pulseEnergy, double waistRadius, Point3D waistPosition)` Constructor. Sets laser wavelength and waist radius.

- `void SetWavelength(double wavelength)`, `void SetWaistRadius(double waistRadius)`, `void SetPulseEnergy(double pulseEnergy)`, `void SetWaistPosition(Point3D waistPosition)` Self-evident.

- `void SetGasVolume(Point3D p1, Point3D p2)` Set limits for the primary ionization function to be later integrated.

- `void CalculateAveragePrimaries()` Calculate the average number of electrons created by a single pulse through Monte Carlo integration and define a Poisson random variable with average equal to the number just calculated.

- `int Pulse()` Call `CalculateAveragePrimaries` if it has not yet been called, then return number sampled from Poisson distribution.

- `Point3D GetPrimaryElectron()` Sample point in the gas volume from the ionization density function.

## `IntegrableFunction`

Describes a real-valued function in 3D space that can be integrated and from which points in space can be sampled.

- `IntegrableFunction()` Constructor, creates a uniform distribution needed later for sampling in 3D space.

- `void IntegrableFunction::SetExpression(std::function<double(T&,Point3D)> function)` Assigns a function to the `functionExpression` private variable.

- `void SetDomain(Point3D p1, Point3D p2)` Set function range in 3D space, needed for Monte Carlo integration.

- `void SetCodomain(double min, double max)` Set maximum and minimum function values, required for rejection sampling.

- `double Evaluate(Point3D p)` Return the value of the lambda function `functionExpression` calculated at `p`.

- `Point3D SamplePoint()` Returns position sampled from `functionExpression` through rejection sampling.

- `double IntegrateMonteCarlo(int nsamples)` Integrate the function `functionExpression` over the previously specified domain with number of Monte Carlo samples equal to `nsamples`.