#! /bin/python
#
# Conditions for using this compiler:
# 1) when importing the API, each method call needs to be imported separately (no `from api import *`, instead `from api import a, b, c`)
# 2) when defining methods in the API, redefine all variables directly within the API method with a unique name (i.e. `top_api_funstuff`). It does not look like I can do cascading variable redefinitions
# 3) Do not put any comments after a line. while the code should still work, it inserts `None` before the comment on a newline
# 4) All API methods to be replaced have to be function calls

from rope.base.exceptions import ResourceNotFoundError, RefactoringError
from rope.refactor import inline
import re
import os.path as op

# todo: get api library location
# todo: get location of code to compile
# todo: copy file that is to be compiled over
# todo: potentially allow a directory to be passed
# todo: if so, copy all files from directory to compiled folder
# todone: dynamically import a list of APIs

def compile_with_top(source_path, api_path):
    source_path = op.abspath(source_path)
    api_path = op.abspath(api_path)
    api_root, api_name = op.split(api_path)
    api_module = op.basename(op.normpath(api_root))
    source_root, source_name = op.split(source_path)
    prefs = {'save_objectdb': False, 'save_history': False,
             'validate_objectdb': False, 'automatic_soa': False,
             'ignored_resources': ['.ropeproject', '*.pyc'],
             'import_dynload_stdmods': False, 'ropefolder': None}

    # Create the two projects that we will be using
    # print source_path, source_root, source_name
    # print api_path, api_root, api_name
    # print op.join(source_root, 'top_compiled')

    from rope.base.project import Project
    project_const = Project(source_root, **prefs)  # original project, used as a source and will not be modified
    project_swap = Project(op.join(source_root, 'top_compiled'), **prefs)

    if(op.isdir(source_path)):
        pass

    # copy over the files that will be compiled
    source_mod = project_const.root.get_child(source_name)
    try:
        mod_code = project_swap.root.get_child(source_name)
    except ResourceNotFoundError:
        mod_code = project_swap.root.create_file(source_name)

    mod_code.write(source_mod.read())

    # Generate the api library package and copy it over
    from rope.contrib import generate
    try:
        api_pkg = project_swap.root.get_child(api_module)
    except ResourceNotFoundError:
        api_pkg = generate.create_package(project_swap, api_module)
    try:
        mod_api = project_swap.root.get_child('test_api/api.py')
    except ResourceNotFoundError:
        mod_api = generate.create_module(project_swap, 'api', api_pkg)
    # source_api = project_const.root.get_child('test_api/api.py')
    # copy over the contents of the api so that rope can modify it
    with open(api_path, 'r') as a:
        api_content = a.read()

    # mod_api.truncate().write(api_content)
    mod_api.write(api_content)

    # inline each API call
    # first, get the list of all API function calls possible
    import imp
    import types
    api = imp.load_source(api_name, api_path)
    api_calls = [a for a in dir(api) if isinstance(api.__dict__.get(a), types.FunctionType)]
    # perform the replacement
    for a in api_calls:
        # indices = [a.start() for a in re.finditer(r'\b{}\b'.format(a), mod_code.read())]
        # if len(indices) > 1:
        #     ind = indices[1]
        #     print ind
        ind = re.search(r'\b{}\b'.format(a), mod_code.read()).start()# + 1
        # print source_mod.read()[ind:ind+5]
        # print project_swap, mod_code, ind
        try:
            inl = inline.create_inline(project_swap, mod_code, ind)
            change = inl.get_changes()
            project_const.do(change)
        except RefactoringError:
            print "The api cannot be properly connected from the code. Please ensure that you are importing the file with the API commands directly."

    # Cleaning up
    api_pkg.remove() # remove the API library from the compiled files
    project_const.close()
    project_swap.close()
