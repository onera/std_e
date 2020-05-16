#module load cmake/3.12.3
#module load gcc/8.3
##module load python/3.6.1
#module load python/2.7.13
##module load cppcheck/1.90-gnu-5.2.0 
#module load CFD/5.0-intel-19.impi-19
source /home/bmaugars/ENVSPIROFORDOCS8.3PY3

export CC=gcc
export CXX=g++

cd ..
#export PROJECT_ROOT=$(pwd)/../..
cmake -S . -Bbuild/ \
  -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_PATH} \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_CXX_FLAGS="-Wall -std=c++17 -fmax-errors=4" \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -DNDEBUG -fopenmp -fPIC" \
  -DSTD_E_ENABLE_TEST=ON \
  -DSTD_E_ENABLE_COVERAGE=ON \
  -DSTD_E_BUILD_DOCUMENTATION=ON && \
#cmake -S . -Bbuild/ \
#  -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_PATH} \
#  -DCMAKE_BUILD_TYPE=Debug \
#  -DCMAKE_CXX_STANDARD=17 \

echo -e "\033[31m" && date && echo -e "\e[0m" && \
#cmake --build build/ -j 24 && \
(cd build && \
make -j24 && \
make docs
)
echo -e "\033[31m" && date && echo -e "\e[0m" && \


## launch unit tests ##
(cd build && ./std_e_unit_tests)
