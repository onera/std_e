module load cmake/3.12.3
module load gcc/9.2

export CC=gcc
export CXX=g++

cd ..
cmake -S . -Bbuild/ &&

cmake --build build/ -- -j24 &&


## launch unit tests ##
(cd build
./std_e_unit_tests)
