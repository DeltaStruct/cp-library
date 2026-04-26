#!/usr/bin/env python3
# from AtCoder Library

import re
import sys
import argparse
from logging import Logger, basicConfig, getLogger
from os import getenv, environ, pathsep
from pathlib import Path
from typing import List, Set, Optional
import os


logger = getLogger(__name__)  # type: Logger


class Expander:
    include = re.compile(r'#\s*include\s*"(.*)"\s*')

    include_guard = re.compile(r'#\s*pragma\s*once\s*')

    def is_ignored_line(self, line) -> bool:
        if self.include_guard.match(line):
            return True
        if line.strip().startswith('//'):
            return True
        return False

    def __init__(self, lib_paths: List[Path]):
        self.lib_paths = lib_paths

    included = set()  # type: Set[Path]
    include_count = {}

    def find_library(self, library_name: str) -> Path:
        for lib_path in self.lib_paths:
            path = lib_path / library_name
            if path.exists():
                return path
        logger.error('cannot find: {}'.format(library_name))
        raise FileNotFoundError()

    def expand_library(self, library_file_path: Path) -> List[str]:
        library_source = open(str(library_file_path)).read()

        if library_file_path in self.included and (self.include_guard.search(library_source)!=None or self.include_count.get(library_file_path,0)>1000):
            logger.info('already included: {}'.format(library_file_path.name))
            return []
        self.included.add(library_file_path)
        self.include_count[library_file_path] = self.include_count.get(library_file_path,0)+1
        logger.info('include: {}'.format(library_file_path.name))

        result = []  # type: List[str]
        result.append("#line 1 \"" + str(library_file_path) + "\"")
        linenum = 0
        for line in library_source.splitlines():
            if self.is_ignored_line(line):
                continue

            linenum += 1

            m = self.include.match(line)
            if m:
                name = m.group(1)
                result.extend(self.expand_library(self.find_library(name)))
                result.append('#line ' + str(linenum) + ' "' + str(library_file_path) + '"')
                continue

            result.append(line)
        return result

    def expand(self, source: str, origname) -> str:
        self.included = set()
        result = []  # type: List[str]
        linenum = 0
        for line in source.splitlines():
            linenum += 1
            m = self.include.match(line)
            if m:
                library_path = self.find_library(m.group(1))
                result.extend(self.expand_library(library_path))
                if origname:
                    result.append('#line ' + str(linenum + 1) + ' "' + origname + '"')
                continue

            result.append(line)
        return '\n'.join(result)


if __name__ == "__main__":
    basicConfig(
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%H:%M:%S",
        level=getenv('LOG_LEVEL', 'INFO'),
    )
    parser = argparse.ArgumentParser(description='Expander')
    parser.add_argument('source', help='Source File')
    parser.add_argument('-f', '--file',
                        action='store_false', help='Print to File')
    parser.add_argument('--lib', help='Path to Library')
    parser.add_argument('--origname', help='report line numbers from the original ' +
                                           'source file in GCC/Clang error messages', default="main.cpp")
    opts = parser.parse_args()

    lib_paths = []
    if opts.lib:
        lib_paths.append(Path(opts.lib))
    if 'CPLUS_INCLUDE_PATH' in environ:
        lib_paths.extend(
            map(Path, filter(None, environ['CPLUS_INCLUDE_PATH'].split(pathsep))))
    lib_paths.append(Path.cwd())
    lib_paths.append(Path.cwd()/"deltastruct")
    expander = Expander(lib_paths)
    source = open(opts.source).read()
    output = expander.expand(source, opts.origname)

    if opts.file:
        print(output)
    else:
        with open('combined.cpp', 'w') as f:
            f.write(output)
            print(os.path.getsize("combined.cpp"),"Bytes")