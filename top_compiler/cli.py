import argparse

def get_arg_parse():
    parser = argparse.ArgumentParser(description='Compile Python for the Triangle Inequality.')
    parser.add_argument('source',
                        metavar='PATH',
                        help='The file path to the file to be compiled')
    parser.add_argument('--library',
                        metavar='PATH',
                        help='The file path to the TOP library',
                        default='./test/api.py')

    return parser