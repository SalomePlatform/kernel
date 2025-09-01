#!/usr/bin/env python3

# THIS SCRIPT IS FOR DEBUGGING PURPOSES ONLY.

# This script collects the runpath information of all shared libraries in a directory
# and saves it to a text file. The runpath information is obtained using the readelf
# command. The script takes a single argument, which is the directory containing the
# shared libraries. If no argument is provided, the current working directory is used.

import os
import sys
import subprocess
from collections import defaultdict

def get_runpath_info(lib_path):
    try:
        result = subprocess.run(['readelf', '-d', lib_path], capture_output=True, text=True, check=True)
        runpath = ""
        for line in result.stdout.splitlines():
            if 'RUNPATH' in line or 'RPATH' in line:
                runpath += line.strip() + "\n"
        return runpath.strip()
    except subprocess.CalledProcessError as e:
        return f"Error reading {lib_path}: {e}"

def search_shared_libs(directory):
    shared_libs = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.so') or '.so.' in file:
                lib_path = os.path.join(root, file)
                shared_libs.append(lib_path)
    return shared_libs

def collect_runpath_info(directory):
    shared_libs = search_shared_libs(directory)
    runpath_info = defaultdict(list)
    for lib in shared_libs:
        runpath_info[os.path.dirname(lib)].append((lib, get_runpath_info(lib)))
    return runpath_info

def save_runpath_info(directory, runpath_info):
    output_file = f"{os.path.basename(directory)}_runpath.txt"
    with open(output_file, 'w') as f:
        for dir_path in sorted(runpath_info.keys()):
            f.write(f"{dir_path}\n{'=' * len(dir_path)}\n")
            for lib, runpath in sorted(runpath_info[dir_path]):
                lib_name = os.path.basename(lib)
                f.write(f"{lib_name}\n{runpath}\n\n")

def main():
    if len(sys.argv) > 1:
        directory = sys.argv[1]
    else:
        directory = os.getcwd()

    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a valid directory.")
        return

    runpath_info = collect_runpath_info(directory)
    save_runpath_info(directory, runpath_info)
    print(f"Runpath information saved to {os.path.basename(directory)}_runpath.txt")

if __name__ == "__main__":
    main()