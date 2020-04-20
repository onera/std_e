module load cmake/3.12.3
module load gcc/9.2

export CC=gcc
export CXX=g++

cd ..
export PROJECT_ROOT=$(pwd)/../..
cmake -S . -Bbuild/ \
  -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_PATH} \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-Wall -std=c++17 -fmax-errors=4" \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -DNDEBUG -fopenmp -fPIC" && \

## build graph ##
cmake --build build/ -j 24 --target &&
#cmake --build build/ -j 24 --target install --config Release &&

## launch unit tests ##
(cd build && \
echo && \
echo "std_e_unit_tests" && \
./external/std_e/std_e_unit_tests && \
echo && \
echo "graph_unit_tests" && \
./graph_unit_tests)
