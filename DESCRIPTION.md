# Class description

## `Laser`

Class to determine the number and position of primary electrons given specific laser parameters (pulse energy, waist radius, wavelength etc.).

- `Laser(double wavelength, double waistRadius)` Constructor. Sets laser wavelength and waist radius.

- `void SetWavelength(double wavelength)`, `void SetWaistRadius(double waistRadius)`, `void SetPulseEnergy(double pulseEnergy)`, `void SetWaistPosition(double x, double y, double z)` Self-evident.

- `void SetGasVolume(double x1, double y1, double z1, double x2, double y2, double z2)` Set limits for the primary ionization function to be later integrated.

- `void Initialize()` Define the laser-induced ionization density as an `IntegrableFunction`, calculate the average number of electrons created by a single pulse through Monte Carlo integration and define a Poisson random variable with average equal to the number just calculated.

- `int Pulse()` Call `Initialize` if it has not yet been called, then return number sampled from Poisson distribution.

- `void GetPrimaryElectron(double &x, double &y, double &z)` Sample point in the gas volume from the ionization density function.

## `IntegrableFunction`

Describes a real-valued function in 3D space that can be integrated and from which points in space can be sampled.

- `IntegrableFunction()` Constructor, creates a uniform distribution needed later for sampling in 3D space.

- `void IntegrableFunction::SetExpression(std::function<double(double,double,double)> function)` Assigns a lambda function to the `functionLambda` private variable.

- `void SetDomain(double x1, double y1, double z1, double x2, double y2, double z2)` Set function range in 3D space, needed for Monte Carlo integration.

- `void SetCodomain(double min, double max)` Set maximum and minimum function values, required for rejection sampling.

- `double Evaluate(double x, double y, double z)` Return the value of the lambda function `functionLambda` calculated at `(x, y, z)`.

- `void SamplePoint(double &x, double &y, double &z)` Set the values of `x`, `y` and `z` to values sampled from `functionLambda` through rejection sampling.

- `double IntegrateMonteCarlo(int nsamples)` Integrate the function `functionLambda` over the previously specified domain with number of Monte Carlo samples equal to `nsamples`.