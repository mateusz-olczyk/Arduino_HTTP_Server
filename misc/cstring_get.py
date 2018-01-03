#!/usr/bin/python3

from sys import argv
from re import match

try:
    with open(argv[1]) as f:
        lines = f.readlines()
except (IndexError, FileNotFoundError):
    print('Script needs valid file name.')
else:
    one_line = '\\n'.join((filter(lambda l: len(l) > 0, map(lambda l: match(r'^\s*(.*)', l).group(1).replace('\\', '\\\\').replace('"', '\\"'),lines))))
    print('const PROGMEM char progmem_string[] = "' + one_line + '";')
