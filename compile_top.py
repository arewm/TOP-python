#! /usr/bin/env python

import top_compiler

if __name__ == '__main__':
    parser = top_compiler.get_arg_parse()
    parsed_args = parser.parse_args()

    top_compiler.compile_with_top(parsed_args.source, parsed_args.library)