fortranCompiler ?= gfortran
gccCompiler ?= gcc
gplusCompiler ?= g++

$(info You are using the following compilers)
$(info Fortran:)
$(info $(shell $(fortranCompiler) -v))
$(info GCC)
$(info $(shell $(gccCompiler) -v))
$(info  G++:)
$(info $(shell $(gplusCompiler) -v))

c_objects = \
    main.o crossprod.o dotprod.o matrixmult.o matxvec.o orbit2inertial.o orbital2state.o propagation.o \
    state2orbital.o invertmat.o load_inputs.o t2doy.o get_density.o angvelprop.o sc_parameters.o sc_geometry.o \
    quatnormalize.o angle2quat.o wind.o quat2dcm.o vectors2angle.o dcm2angle.o norm.o aero_torque.o aero_force.o \
    gravity_field.o norm_coef.o gaus_coef.o magnet_field.o eddy_torque.o aero_drag.o nrlmsise-00.o nrlmsise-00_data.o \
    gaus_coef_wmm.o magnet_field_wmm.o days2mdh.o  precess.o nutation.o sidereal.o teme2ecef.o ecef2lla.o \
    transpose.o load_teme.o polarm.o moon.o sun.o third_body.o check_inputs.o tt2utc.o grav_potential.o srp.o srp_force.o \
    shadow_function.o albedo_calc.o sun_potential.o moon_potential.o ecef2eci.o

cpp_objects = tle2rv_exec

fortran_objects = JB2008.o hwm14.o

c_executables = \
    JB2008.for hwm14.f90 main.c crossprod.c dotprod.c matrixmult.c matxvec.c orbit2inertial.c \
    orbital2state.c propagation.c state2orbital.c get_density.c invertmat.c load_inputs.c t2doy.c angvelprop.c \
    sc_parameters.c sc_geometry.c quatnormalize.c angle2quat.c wind.c quat2dcm.c vectors2angle.c dcm2angle.c norm.c \
    aero_torque.c aero_force.c gravity_field.c norm_coef.c gaus_coef.c magnet_field.c eddy_torque.c aero_drag.c \
    nrlmsise-00.c nrlmsise-00_data.c gaus_coef_wmm.c magnet_field_wmm.c days2mdh.c precess.c nutation.c sidereal.c \
    teme2ecef.c ecef2lla.c transpose.c load_teme.c polarm.c moon.c sun.c third_body.c check_inputs.c tt2utc.c srp.c \
    srp_force.c shadow_function.c albedo_calc.c grav_potential.c sun_potential.c moon_potential.c ecef2eci.c

cpp_executables = tle2rv.cpp SGP4.cpp

fortran_executables = JB2008.for hwm14.f90

tle2rv_exec: $(addprefix src/cpp/,$(cpp_executables))
	$(gplusCompiler) $(addprefix src/cpp/,$(cpp_executables)) -o tle2rv_exec

src/JB2008.o: src/fortran/JB2008.for
	$(fortranCompiler) -J src -c src/fortran/JB2008.for && mv JB2008.o src/

src/hwm14.o: src/fortran/hwm14.f90
	$(fortranCompiler) -J src -c src/fortran/hwm14.f90 && mv hwm14.o src

src/%.o: src/c-transforms/%.c
	$(gccCompiler) -std=gnu99 -c $< -o $@

dspose_exec: $(addprefix ,$(cpp_objects)) $(addprefix src/,$(fortran_objects)) $(addprefix src/,$(c_objects))
	$(gccCompiler) $(addprefix src/,$(c_objects)) $(addprefix src/,$(fortran_objects)) -lgfortran -o dspose_exec

test: dspose_exec tle2rv_exec
	./tle2rv_exec
	./dspose_exec
	rm -r src/*.o src/*.mod

clean:
	rm -r src/*.o src/*.mod dspose_exec tle2rv_exec