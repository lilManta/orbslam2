echo "Configuring and building Thirdparty/DBoW2 ..."

cd Thirdparty/DBoW2
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/ywz/work/3tparty/opencv-4.2.0/build
make -j16

cd ../../g2o

echo "Configuring and building Thirdparty/g2o ..."
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/ywz/work/3tparty/build/ubuntu/Release/share/eigen3/cmake
make -j16

cd ../../../

echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

echo "Configuring and building ORB_SLAM2 ..."

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j16
