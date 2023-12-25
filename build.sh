if [ -d "build" ]; then
    cd build
else
    mkdir "build"
    cd build
fi

cmake ..

if [[ $? -eq 0 ]]
then
	make
fi
mv queue ..
rm -rf ../build