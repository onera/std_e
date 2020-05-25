source /home/bmaugars/ENVSPIROFORDOCS8.3PY3
export PATH=/scratchm/bmaugars/opt-doxygen/bin:$PATH
#pip3 install sphinxcontrib-applehelp --user
#pip3 install breathe==4.17 --user

#export PATH=/scratchm/bmaugars/opt-cppcheck/bin/:$PATH
#module load gcc/5.2 cppcheck/1.90-gnu-5.2.0

export CC=gcc
export CXX=g++

cd ..
export PROJECT_ROOT=$(pwd)/../..
#cmake -S . -Bbuild/ \
#  -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_PATH} \
#  -DCMAKE_BUILD_TYPE=Debug \
#  -DCMAKE_CXX_STANDARD=17 \
#  -DCMAKE_CXX_FLAGS="-Wall -std=c++17 -fmax-errors=4" \
#  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -DNDEBUG -fopenmp -fPIC" \
#  -DSTD_E_ENABLE_TEST=ON \
#  -DSTD_E_ENABLE_COVERAGE=OFF \
#  -DSTD_E_BUILD_DOCUMENTATION=ON \
#  -DCMAKE_EXPORT_COMPILE_COMMANDS=OFF \
#  && \
#cmake -S . -Bbuild/ \
#  -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_PATH} \
#  -DCMAKE_BUILD_TYPE=Debug \
#  -DCMAKE_CXX_STANDARD=17 \

echo -e "\033[31m" && date && echo -e "\e[0m" && \
#cmake --build build/ -j 24 && \
(cd build && \
#make clean && \
make -j24 && \
#make doc
#make std_e_coverage
#cppcheck --enable=all --project=compile_commands.json --xml
echo "make done"
) && \
echo -e "\033[31m" && date && echo -e "\e[0m" && \


## launch unit tests ##
(cd build && ./std_e_unit_tests)
