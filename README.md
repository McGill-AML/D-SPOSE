# D-SPOSE: Debris SPin/Orbit Simulation Environment

## About the author
Luc Sagnières, 2018  
McGill University  
Aerospace Mechatronics Laboratory  
luc.sagnieres@mail.mcgill.ca

## General info
```
Version: 1.0.0
Release Date: January 15, 2019
```

READ [DOCUMENTATION](docs) BEFORE USE.

A useful source is the Software User Manual and other documentation in the [lab repo](https://github.com/McGill-AML).

## Prerequisites

* You will need [GFortran](https://gcc.gnu.org/wiki/GFortran) to execute the code contained in this repo.
Follow [these instructions](https://gcc.gnu.org/wiki/GFortranBinaries) to install it in your environment.
A stable recommended release is recommended (at least `8.1.0`)
* If you are working on MacOS, you'll have to link it manually
    ```bash
    ln -s /usr/local/Cellar/gcc/8.2.0/lib/gcc/8/libgfortran.* /usr/local/lib
    ```
* We are leveraging the GNU functionalities of `make` with a version `>4`.
To install it on make you can do `brew install homebrew/core/make` and use it with calling `gmake`
instead of `make`. `make` will remain with the default older version on mac `3.81`
and `gmake` will be the newer version.

## Testing on your system

```bash
make test
```
It will print out your compiler information. You can put your own compiler as such:
```bash
fortranCompiler=myFortranCompiler gplusCompiler=myg++Compiler gccCompiler=myGccCompiler make test
```
Otherwise there are default compilers defined in the `Makefile`.

This should create in the [output folder](output) the files that should be equal to the [reference files](output/expected-output)

```bash
make clean
```
This will clean the test folder of the created `.mod`, `.o` and `executable` files.

## Running D-SPOSE

The two compiled `executable` files can then be used freely by the user to run D-SPOSE according to the input text files in the [intput foler](input). 

The first converts the Two-Line Element contained in `TLE.txt` into an osculating position and velocity vector contained in `initial_conditions.txt` using the SGP4 algorithm:
```bash
./tle2rv_exec
```

The second runs the coupled orbit-attitude propagator using the parameters in the other input files in the [intput foler](input) and outputs the information in the [output folder](output):
```bash
./dspose_exec
```

## REFERENCES

1. Textbooks:
   * Vallado D.A. (2013), Funamentals of Astrodynamics and Applications, Fourth Edition, Microcosm Press. ISBN 978-1881883180.
   * Hughes, P.C. (2004), Spacecraft Attitude Dynamics, Dover Publications, Inc.  ISBN 978-0-486-43925-9.
   * Montenbruck, O., and Gill, E. (2012), Satellite Orbits: Models, Methods, Applications, Springer-Verlag Berlin Heidelberg. ISBN: 978-3-540-67280-7.

2. SGP4:
   * Vallado, D. A., P. Crawford, R. Hujsak, and T. S. Kelso (2006), Revisiting Spacetrack Report #3: Rev 2.

3. Environmental Models:
   * Bowman, B. R. (2008), A New Empirical Thermospheric Density Model JB2008 Using New Solar and Geomagnetic Indices, In Proceedings  of  the  AIAA/AAS  Astrodynamics  Specialist  Conference  and  Exhibit,  AIAA  2008-6438,  Honolulu,  Hawaii, doi:10.2514/6.2008-6438.
   * Bruinsma, S. (2015), The DTM-2013 thermosphere model, J. Space Weather Space Clim., 5, A1, doi:10.1051/swsc/2015001.
   * Drob, D. P. et al. (2015), An update to the Horizontal Wind Model (HWM): The quiet time thermosphere, Earth and Space Science, 2:301–319, 2015. doi:10.1002/2014EA000089.
   * Pavlis, N. K., S. A. Holmes, S. C. Kenyon, and J. K. Factor (2012), The development and evaluation of the Earth Gravitational Model 2008 (EGM2008), Journal of Geophysical Research, 117:B04406, 2012. doi:10.1029/2011JB008916.
   * Picone, J. M., A. E. Hedin, D. P. Drob, and A. C. Aikin (2002), NRLMSISE-00 empirical model of the atmosphere: Statistical comparisons and scientific issues, Journal of Geophysical Research, 107(A12):1468, doi:10.1029/2002JA009430.
   * Thébault, E. et al. (2015), International Geomagnetic Reference Field: the 12th generation, Earth Planet Sp, 67(79), doi:10.1186/s40623-015-0228-9.
   * Chulliant, A. et al. (2015), Tehcnical  Report, National  Geophysical  Data Center, NOAA, doi:10.7289/V5TB14V7.
   * Kane, T., and Barba, P. (1966), “Effects of Energy Dissipation on a Spinning Satellite,” AIAA Journal, Vol. 4, 1392–1394, doi:10.2514/3.3683.
   * Stephens, G. L., Campbell, G. G., and Vonder Haar, T. H. (1981), Earth Radiation Budgets, Journal of Geophysical Research, Vol. 86, pp. 9739-9760. doi:10.1029/JC086iC10p09739.

4. Input Data:
   * [Two-Line Elements](http://celestrak.com/)
   * [Earth Gravitational Model 2008](http://earth-info.nga.mil/GandG/wgs84/gravitymod/egm2008/egm08_wgs84.html)
   * [World Magnetic Model](https://www.ngdc.noaa.gov/geomag/WMM/wmm_ddownload.shtml)
   * [International Geomagnetic Reference Field 12th Edition](https://www.ngdc.noaa.gov/IAGA/vmod/igrf.html)
   * [Drag Temperature Model 2013](http://www.atmop.eu/index.php/models)
   * [Jacchia-Bowman 2008](http://sol.spacenvironment.net/jb2008/code.html)
   * [Horizontal Wind Model 2014](https://github.com/pa345/lib/tree/master/hwm)
   * [NRLMSISE-2000](https://github.com/magnific0/nrlmsise-00)
   * [Geomagnetic Indices (Ap and Kp)](https://www.ngdc.noaa.gov/stp/GEOMAG/kp_ap.html)
   * [Solar Indices](ftp://ftpsedr.cls.fr/pub/previsol/solarflux/)
   * [Earth Orientation Parameters](https://www.iers.org/IERS/EN/DataProducts/EarthOrientationData/eop.html)
   * [Sun and Moon Ephemeris](http://vo.imcce.fr/webservices/miriade/)
