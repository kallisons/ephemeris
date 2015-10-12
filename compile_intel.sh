# Script to compile on MacBook Pro with Intel fortran compiler
# 27 Jan 2007

# clean up the shop
rm *.o

# compile source 
ifort -m64 -c jplint.f ephem.f -assume nounderscore
gcc -c novas.c
gcc -c readeph0.c
gcc -c novascon.c
gcc -c solsys2.c
gcc -c suneph_args.c

# link objects together into a single execuatable (-nofor_main flag means the main program is not written in FORTRAN)
ifort -m64 -nofor-main suneph_args.o novas.o readeph0.o novascon.o solsys2.o ephem.o jplint.o -nofor-main -o eph

