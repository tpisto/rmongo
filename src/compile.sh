cd mongo-cxx-driver

# PCRE
cd pcre
./configure
make
cd ..

# BOOST
cd boost
./bootstrap.sh
./bjam
cd ..

# MONGO
scons
cd ..

# MONGO R
make
