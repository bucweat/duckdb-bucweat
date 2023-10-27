# System.Data.ODBC tests for DuckDB ODBC

Written in Microsoft C++/CLI using .NET Framework 4.8.1.

Cmake Visual Studio Generator is required to build due to use of Microsoft C++/CLI.

Test app is built as part of a standard DuckDB build and used by Windows CI.

After successful build, the test executable can be found in:

> [build directory]\release\tools\odbc\bin\Release\SystemDataODBC_tests.exe

Uses the catch framework.

## Test Development

In order to facilitate dotnet test development, a batch file is provided 
to build the test exe locally. `build.bat` will discover the version of
Visual Studio installed on your workstation and will then execute cmake
to configure and build the test executable. 

To build, open a standard cmd window and run `build.bat`. 


```
>build.bat
Using Visual Studio 2017 / 2019 / 2022...
cl.exe compiler architectue is x64
-- The CXX compiler identification is MSVC 19.37.32822.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (3.7s)
-- Generating done (0.1s)
-- Build files have been written to: E:/github/duckdb-bucweat/tools/odbc/test/tests/dotnet/build
MSBuild version 17.7.2+d6990bcfa for .NET Framework

  Checking Build System
  Building Custom Rule E:/github/duckdb-bucweat/tools/odbc/test/tests/dotnet/CMakeLists.txt
  odbcExampleCpp.cpp
  AssemblyInfo.cpp
  pch.cpp
  Generating Code...
  SystemDataODBC_tests.vcxproj -> E:\github\duckdb-bucweat\tools\odbc\test\tests\dotnet\build\Release\SystemDataODBC_tests.exe
  Building Custom Rule E:/github/duckdb-bucweat/tools/odbc/test/tests/dotnet/CMakeLists.txt
```

To execute tests, run `build\Release\SystemDataODBC_tests.exe`.


Once you've built with the batch file, you'll have a local build folder. Included 
in this folder is a Visual Studio solution file. The solution file allows you to
write code, debug and test the application in the Visual Studio IDE. 

Open the solution with Visual Studio, right click on `SystemDataODBC_tests` and 
select `Set as Startup Project`. Now you can `F5` to build and debug the test app.
