if exist build\ (
    del /s /q build\*
    rmdir /s /q build\
)
mkdir build\
cd build

cmake -G "Visual Studio 16 2019" -A x64 ..
