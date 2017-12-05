#! /bin/python
#
# Conditions for using this compiler:
# 1) when importing the API, each method call needs to be imported separately (no `from api import *`, instead `from api import a, b, c`)
# 2) when defining methods in the API, redefine all variables directly within the API method with a unique name (i.e. `top_api_funstuff`). It does not look like I can do cascading variable redefinitions
# 3) Do not put any comments after a line. while the code should still work, it inserts `None` before the comment on a newline
# 4) have a list containing the names of all API calls that will be replaced

from rope.base.exceptions import ResourceNotFoundError
from rope.refactor import inline
import re

# todo: get api library location
# todo: get location of code to compile
# todo: copy file that is to be compiled over
# todo: potentially allow a directory to be passed
# todo: if so, copy all files from directory to compiled folder
# todo: dynamically import a list of APIs

def compile_with_top(source_path, api_path):
    prefs = {'save_objectdb': False, 'save_history': False,
             'validate_objectdb': False, 'automatic_soa': False,
             'ignored_resources': ['.ropeproject', '*.pyc'],
             'import_dynload_stdmods': False, 'ropefolder': None}

    # Create the two projects that we will be using
    from rope.base.project import Project
    project = Project('.', **prefs)
    project2 = Project('./top_compiled', **prefs)

    # copy over the files that will be compiled
    source_mod = project.root.get_child('tester.py')
    try:
        mod1 = project2.root.get_child('tester_compiled.py')
    except ResourceNotFoundError:
        mod1 = project2.root.create_file('tester_compiled.py')

    mod1.write(source_mod.read())

    # Generate the api library package and copy it over
    from rope.contrib import generate
    try:
        pkg = project2.root.get_child('test_api')
    except ResourceNotFoundError:
        pkg = generate.create_package(project2, 'test_api')
    try:
        mod2 = project2.root.get_child('test_api/api.py')
    except ResourceNotFoundError:
        mod2 = generate.create_module(project2, 'api', pkg)
    source_api = project.root.get_child('test_api/api.py')
    mod2.write(source_api.read())

    # inline each API call
    api_calls = ['REPLACE_ME', 'REPLACE_ME_TOO']
    for a in api_calls:
        inl = inline.create_inline(project2, mod1, re.search(r'\b{}\b'.format(a), mod1.read()).start() + 1)
        change = inl.get_changes()
        project.do(change)

    # Cleaning up
    pkg.remove() # remove the API library from the compiled files
    project.close()
    project2.close()
