import top_compiler

if __name__ == '__main__':
    parser = top_compiler.get_arg_parse()
    parser.parse_args()

    top_compiler.compile_with_top(parser.source, parser.library)