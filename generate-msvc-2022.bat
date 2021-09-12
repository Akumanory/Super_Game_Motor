if exist build\ (
    del /s /q build\*
    rmdir /s /q build\
)
mkdir build\
cd build

cmake -G "Visual Studio 17 2022" -A x64 -T v143 ..
