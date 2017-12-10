##############

CODE STRUCTURE

##############

All code for the compiler is contained with the folder "top_compiler" except for the script to run the tool.

File: `cli.py`
    This file contains the code to generate the argument parser for the command line interface.

File: `compiler.py`
    This file contains the main compiler code. It has a single function which is called with the file paths for the
    code to be compiled and the API location. The compiler leverages the rope package to inline function calls from
    the API into the code.

File: `tester.py`
    This is a simple test file that can be used to check the functionality of the compiler.

Folder: `test_api`
    This folder contains the test api that can be used for compiling "tester.py."

############

INSTALLATION

############

Use of this compiler has two preconditions:
- Python 2.7.14
- rope 0.10.7 (Python package)

To install Python 2.7.14 on a VCL image:

To install rope:
    pip install rope


#########

EXECUTION

#########

Execution of the compiler can be accomplished with the following command:
    python compile_top.py <source_file.py> [--library <api_file.py>]

If you do not provide a library, the default API will be used.

In order for the compiler to work properly on a Python file, it must have the following property:
- when importing the API, each method call needs to be imported separately (no `from api import *`, instead `from api import a, b, c`)

If defining a new API for the compiler to replace, it must have the following property:
- when defining methods in the API, redefine all variables directly within the API method with a unique name (i.e. `top_api_funstuff`). It does not look like I can do cascading variable redefinitions
- All API methods to be replaced have to be function calls


###############

COMPILER OUTPUT

###############

Upon completion of compiling, the modified code will be placed within a `top_compiled` folder at the same level as the
source file input to the compiler. All imported method calls from the API will be replaced with the API code and the
import calls will be removed.

If there are any issues with the compilation process, an error will be thrown. If any compilation occurred before the
failure, the partially compiled file will be located within the `top_compiled` folder.

#########

TEST CASE

#########

To execute the test case, from the current directory, use the command:
    python compile_top.py top_compiler/tester.py --library top_compiler/test_api/api.py

This will create a `top_compiled` folder within `top_compiler` containing the compiled tester.py file.