#!/bin/sh

# Wraps the process of getting dependencies during CI.
#
# Usage:
#   get_dependencies [<depend1> [<depend2> [...]]]
#
# Optional Arguments:
#   depend1: name of the first dependency
#   depend2: name of the second dependency
# Other variables:
#   cmake_version: the version of cmake to get in the format x.y.z
#
# Notes:
#   - This script needs to be run in sudo mode to actually install dependencies
#   - Python dependencies are installed in the virtual environment "venv"

################################################################################
#                               Internal Functions                             #
################################################################################

# Wraps downloading and installing a specific version of CMake
#
# Usage:
#   get_cmake <cmake_version>
# Arguments:
#   cmake_version: The version of CMake to get in the format x.y.z
get_cmake() {
  cmake_url_base="https://github.com/Kitware/CMake/releases/download"
  arch="Linux-x86_64"
  script_name="cmake-${1}-${arch}.sh"
  wget "${cmake_url_base}/v${1}/${script_name}"
  yes | /bin/sh "${script_name}"
}

# Wraps installing Doxygen
#
# Usage:
#   get_doxygen
get_doxygen() {
  sudo apt update
  sudo apt-get install -f doxygen
}

# Wraps installing Sphinx and the ReadTheDocs Theme
#
# Usage:
#   get_sphinx
get_sphinx() {
  sudo apt update
  sudo apt-get install -f python3-venv
  python3 -m venv venv
  source venv/bin/activate
  pip3 install sphinx sphinx_rtd_theme
}

################################################################################
#                               Main Script                                    #
################################################################################

for depend in "$@"; do
  echo "Getting dependency: ${depend}"
  # Please use camel_case for dependency names and keep the if-statements in
  # alphabetical order.
  if [ "${depend}" = "cmake" ]; then
    get_cmake ${cmake_version}
  elif [ "${depend}" = "doxygen" ]; then
    get_doxygen
  elif [ "${depend}" = "sphinx" ]; then
    get_sphinx
  else
    echo "Unrecognized dependency: ${depend}"
    exit 99
  fi
done
