# This script builds and automatically packs builds for you

echo "Deleting all files from ./releases folder"
rm releases/*

#echo "Compiling app for all OSes"
#./allplatforms.sh

echo "Copying all assets to output folder"
cp mf output -r
cp assets output -r

# Linux
echo "Packing for Linux"
mkdir mfbuild
cp output/app mfbuild
cp output/mf mfbuild -r
cp output/assets mfbuild -r
cp output/saves mfbuild -r

cd releases
7z a linux.zip ../mfbuild
cd ..
rm mfbuild -r

# Win x86_64
echo "Packing for 64bit Windows (aka x86_64)"
mkdir mfbuild
cp output/app-x86_64.exe mfbuild
cp output/mf mfbuild -r
cp output/assets mfbuild -r
cp output/saves mfbuild -r
cp output/x86_64dll/* mfbuild

cd releases
7z a win-x86_64.zip ../mfbuild
cd ..
rm mfbuild -r

# Win x86
echo "Packing for 32bit Windows (aka x86)"
mkdir mfbuild
cp output/app-x86.exe mfbuild
cp output/mf mfbuild -r
cp output/assets mfbuild -r
cp output/saves mfbuild -r
cp output/x86dll/* mfbuild

cd releases
7z a win-x86.zip ../mfbuild
cd ..
rm mfbuild -r

# Finish
echo "Finished! Check result in ./releases directory"
