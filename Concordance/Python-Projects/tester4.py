#!/usr/bin/env python

#
# UVic SENG 265, Summer 2021, Assignment #4
#
# This test-driver program invokes methods on the class to be
# completed for the assignment.
# 

import sys
import argparse
from concord4 import Concordance


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', type=str, help='exception-word file')
    parser.add_argument('infile', type=str, help='input text file')

    args = parser.parse_args()

    if not args.infile:
        print("Need an infile...")
        sys.exit(1)

    #saved_stdout, saved_stderr = sys.stdout, sys.stderr
    #sys.stdout, sys.stderr = None, None

    c = Concordance(args.infile, args.e)
    result = c.full_text()

    #sys.stdout, sys.stderr = saved_stdout, saved_stderr

    #if result != []:
    #    print("\n".join(result))


if __name__ == "__main__":
    main()
