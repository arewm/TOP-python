import argparse
import os.path as op

api = op.join(op.dirname(__file__), '../api.py')

def get_arg_parse():
    parser = argparse.ArgumentParser(description='Compile Python for the Triangle Inequality.')
    parser.add_argument('source',
                        metavar='PATH',
                        help='The path to the file to be compiled')
    parser.add_argument('--library',
                        metavar='PATH',
                        help='The path to the TOP API',
                        default=api)
                        #default='./test/api.py')

    return parser