[![Build status](https://dev.azure.com/davidebeatrici/toggle-nic/_apis/build/status/CI?branchName=master)](https://dev.azure.com/davidebeatrici/toggle-nic/_build/latest?definitionId=4&branchName=master)

## Usage
List interfaces:
```batch
toggle-nic --list
```

Disable interface #11:
```batch
toggle-nic 11 --disable
```

Enable interface #11:
```batch
toggle-nic 11 --enable
```

## Build instructions

#### Visual Studio 2017+

1. Launch Visual Studio and open the repository, the IDE will automatically detect the CMake project.

2. Build the project.

#### nmake

1. Open a command prompt window inside the repository, with the MSVC environment initialized.
You can do that by opening one of the `Tools Command Prompt for VS` shortcuts under the Visual Studio folder in the Start menu.
The shortcuts point to the `.bat` files that can be found in `VC/Auxiliary/Build`, inside the Visual Studio installation directory.
You can also open a "clean" command prompt window and run one of the scripts (depending on the architecture you want to build the project for).
For example, if you want to build a 64 bit binary and have Visual Studio 2019 Community, you would have to run:
```
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
```

The path differs if you have a different Visual Studio version (e.g. Professional) or if you changed it during the installation.


2. Create a working directory for CMake and enter it:
```batch
mkdir build && cd build
```

3. Run CMake with the `NMake Makefiles` generator (otherwise it generates a Visual Studio solution):
```batch
cmake -G "NMake Makefiles" ..
```

4. Build:
```batch
nmake
```

#### MSYS2

1. Open either `MSYS2 MinGW 32-bit` or `MSYS2 MinGW 64-bit` and go inside the repository.

2. Create a working directory for CMake and enter it:
```bash
mkdir build && cd build
```

3. Run CMake:
```bash
cmake -G "MSYS Makefiles" ..
```

4. Build:
```bash
mingw32-make
```
