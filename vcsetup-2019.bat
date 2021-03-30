rmdir /s /q build
mkdir build

cd build


rem Get path to torch cmake folder 
rem assume (mini)conda python environment
FOR /F "delims=" %%i IN ('python -c "import site; print (site.getsitepackages()[0])"') DO set TORCH_INSTALLED_DIR=%%i\lib\site-packages\torch\share\cmake\Torch

echo "TORCH_DIR = %TORCH_INSTALLED_DIR%"

rem Set -DPYTHON_EXECUTABLE=<path/to/python> if you want to specify the path to python.exe manually

cmake -G "Visual Studio 16 2019" -A x64 ^
  -DTorch_DIR=%TORCH_INSTALLED_DIR% ^
  -DCMAKE_BUILD_TYPE=Debug ^
  ..
