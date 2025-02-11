# Copyright (C) 2015-2026  CEA, EDF, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

import argparse
import os
import shutil
import subprocess
from pathlib import Path
import tempfile

from salomeContextUtils import SalomeContextException  # type: ignore # @UnresolvedImport


def __configureTests(args=None, exe=None):
    if args is None:
        args = []
    if exe:
        usage = "Usage: %s [options]" % exe
    else:
        usage = "Usage: %prog [options]"
    epilog = """
Run tests of SALOME components provided with application.
Principal options are:
    -h,--help
        Show this help message and exit.

    --print-labels
        Print the list of all labels associated with the test set.
        This option will not run any tests.

    -V,--verbose
        Enable verbose output from tests.
    -VV,--extra-verbose
        Enable more verbose output from tests.
    -Q,--quiet
        Suppress all output.

    -N,--show-only
        Show available tests (without running them).

    -R <regex>, --tests-regex <regex>
        Run tests matching regular expression.
    -E <regex>, --exclude-regex <regex>
        Exclude tests matching regular expression.

    -L <regex>, --label-regex <regex>
        Run tests with labels matching regular expression.
    -LE <regex>, --label-exclude <regex>
        Exclude tests with labels matching regular expression.

For complete description of available options, pleaser refer to ctest documentation.
"""
    if not args:
        return argparse.Namespace(run_dir=None), []

    parser = argparse.ArgumentParser(
        usage=usage + epilog, epilog="Others options are passed to ctest"
    )
    parser.add_argument(
        "--run-dir",
        nargs='?',
        const="run_in_default",
        default=None,
        help="directory where ctest will be run (write access is required). If used without a path, runs in the default directory.",
    )
    return parser.parse_known_args(args)


# tests must be in ${ABSOLUTE_APPLI_PATH}/${__testSubDir}/
__testSubDir = "bin/salome/test"


def runTests(args, exe=None):
    absolute_appli = os.getenv("ABSOLUTE_APPLI_PATH")
    if not absolute_appli:
        raise SalomeContextException(
            "Unable to find application path. Please check that the variable ABSOLUTE_APPLI_PATH is set."
        )
    absolute_appli_path: Path = Path(absolute_appli)
    testPath = absolute_appli_path / __testSubDir

    cfg, args = __configureTests(args, exe)

    run_dir_arg = cfg.run_dir

    if run_dir_arg and run_dir_arg != "run_in_default":
        run_dir: Path = Path(run_dir_arg)
        run_dir.mkdir(parents=True, exist_ok=True)
        testPath = run_dir

    # The SALOME_TESTS_PATH env variable contains a list of ctest directories separated with a colon.
    # We add these directories in a CTest test file in the test directory.
    # If not test directory was given, we use a temporary one.
    salome_tests_path = os.getenv("SALOME_TESTS_PATH")
    if salome_tests_path:
        salome_tests_path_list = [
            f"subdirs({path})" for path in salome_tests_path.split(":")
        ]
        if not run_dir_arg:
            with tempfile.NamedTemporaryFile() as f:
                testPath = Path(f.name)
            testPath.mkdir()
        ctest_file = testPath / "CTestTestfile.cmake"
        ctest_content = ""
        if ctest_file.is_file():
            ctest_content = ctest_file.read_text()

        ctest_content = "\n".join(salome_tests_path_list)
        ctest_file.write_text(ctest_content)
        ctest_custom_file = testPath / "CTestCustom.cmake"
        if not ctest_custom_file.exists():
            ctest_custom_file.write_text("""set(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE 1048576) # 1MB
set(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 1048576) # 1MB
""")

    if run_dir_arg and run_dir_arg != "run_in_default" and not salome_tests_path:
        appli_dir = Path(absolute_appli_path).parent
        prefix = appli_dir.parents[2]
        for path in testPath.glob("CTest*"):
            if not path.is_file():
                continue
            content = path.read_text()
            content = content.replace("../../../../../../..", str(prefix))
            content = content.replace("../../..", str(appli_dir))
            run_dir.joinpath(path.name).write_text(content)

    command = ["ctest"] + args
    p = subprocess.Popen(command, cwd=testPath)
    p.communicate()
    if salome_tests_path and not run_dir_arg:
        shutil.rmtree(testPath)
    return p.returncode
