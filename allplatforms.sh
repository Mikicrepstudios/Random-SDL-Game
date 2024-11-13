# Ignore this, this is script which I use for every release to compile to every platform, you are not supposed to run this

make -j $(nproc) && make windows -j $(nproc) && make windows32 -j $(nproc) && exit