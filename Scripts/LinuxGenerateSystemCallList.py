#!/usr/bin/env python2

import re

def main(args):
    for name in re.findall (r'asmlinkage long sys_([a-z0-9_]+)', args.file.read ()):
        print args.output_format.format(name)

if __name__ == '__main__':
    from sys import stdin
    import argparse

    parser = argparse.ArgumentParser()

    parser.add_argument ('file', type=argparse.FileType('rb'), default=stdin, metavar='file')
    parser.add_argument ('-f', '--output-format', type=str, default='{}')

    main (parser.parse_args ())
