README.txt

Authors:
- Brian Clee (unity ID: bpclee; GitHub ID: cleebp)
- Andrew McNamara (unity ID: ajmcnama; GitHub ID: arewm)
- Esha Sharma (unity ID: esharma2; GitHub ID: eshasharma)

Python TOP Compiler for distance-related problems, an extension of Ding et al.'s work for our CSC 512 final
group project for the Fall 2017 semester.

##############

CODE STRUCTURE

##############

!!!!!!!!!!!!!!!!!!!!!
!!! Compiler Code !!!
!!!!!!!!!!!!!!!!!!!!!

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

!!!!!!!!!!!!!!!!
!!! API Code !!!
!!!!!!!!!!!!!!!!

All code for the TOP API is defined and included in the file "api.py" which is located in our root directory, there is also
a copy of this file inside the /test_cases folder (however they are identical).

File: `api.py`
    This file contains all TOP API definitions to be used by our compiler in replacing any TOP calls within test cases.
    It is located conviently in root for inspection, but it is also located inside /test_cases/TOP_api where it is actually used.

!!!!!!!!!!!!!!!!!!
!!! Test Cases !!!
!!!!!!!!!!!!!!!!!!

All code for our test cases is located inside the "\test_cases" directory off root. Here one will find 3 different Python test
cases which implement distance-related problems. There are two versions of each test case, an original and a TOP version which is
denoted with the suffix "_TOP".

File: `iris.data`
    The standard iris dataset which we use for Test Case 2.

File: `test_case_1.py`
    Implementation of the P2P distance-related problem. This version is the original standard implementation.

File: `test_case_1_TOP.py`
    Implementation of the P2P distance-related problem. This version is the modified TOP version with calls to our TOP API.

File: `test_case_2.py`
    Implementation of the KNN distance-related problem. This version is the original standard implementation.

File: `test_case_2_TOP.py`
    Implementation of the KNN distance-related problem. This version is the modified TOP version with calls to our TOP API.

File: `test_case_3.py`
    Implementation of the K-Means distance-related problem. This version is the original standard implementation.

File: `test_case_3_TOP.py`
    Implementation of the K-Means distance-related problem. This version is the modified TOP version with calls to our TOP API.

Folder: `TOP_api`
    Contains copy of "api.py" to be used by code, along with compiled versions of the api, "api.pyc"

############

INSTALLATION

############

Use of this compiler has two preconditions:
- Python 2.7.14
- rope 0.10.7 (Python package)

To install Python 2.7.14 on a VCL image:
    follow this guide for installing Python 2 on linux: (http://docs.python-guide.org/en/latest/starting/install/linux/)

To install rope:
    pip install rope

#########

EXECUTION

#########

Execution of the compiler can be accomplished with the following command (from the current directory):
    python compile_top.py <source_file.py> [--library <api_file.py>]

If you do not provide a library, the default API will be used.

In order for the compiler to work properly on a Python file, it must have the following property:
- when importing the API, each method call needs to be imported separately (no `from api import *`, instead `from api import a, b, c`)

If defining a new API for the compiler to replace, it must have the following property:
- when defining methods in the API, redefine all variables directly within the API method with a unique name (i.e. `top_api_funstuff`). It does not look like I can do cascading variable redefinitions
- All API methods to be replaced have to be function calls
- The code for the API must be contained within its own module (see the `test_cases` directory for an example), but the .py function is what needs to be referenced in the CLI


###############

COMPILER OUTPUT

###############

Upon completion of compiling, the modified code will be placed within a `top_compiled` folder at the same level as the
source file input to the compiler. All imported method calls from the API will be replaced with the API code and the
import calls will be removed.

If there are any issues with the compilation process, an error will be thrown. If any compilation occurred before the
failure, the partially compiled file will be located within the `top_compiled` folder.

##################

COMPILER TEST CASE

##################

To execute the compiler test case, from the current directory, use the command:
    python compile_top.py top_compiler/tester.py --library top_compiler/test_api/api.py

This will create a `top_compiled` folder within `top_compiler` containing the compiled tester.py file.

To test compiling other test cases than our example "tester.py" simply replace "tester.py" with the test cases inside "/test_cases" described above.

A faster simpler way of testing the test cases without compiling is to navigate to the "/test_cases" directory and simply run the cases using Python as:
    python test_case_X.py
... where X is replaced with the specific test case you wish to run.
