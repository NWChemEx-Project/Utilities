def repo_name="UtilitiesEx"
def depends = ['NWChemExBase']

def compile_repo(depend_name, install_root, do_install) {
    sh """
        set +x
        source /etc/profile
        module load gcc/7.1.0
        module load cmake
        build_tests="True"
        make_command=""
        if [ ${do_install} == "True" ];then
            build_tests="False"
            make_command="install"
        fi
        cmake -H. -Bbuild -DBUILD_TESTS=\${build_tests} \
                          -DCMAKE_INSTALL_PREFIX=${install_root}\
                          -DCMAKE_PREFIX_PATH=${install_root}
        cd build && make \${make_command}
    """
}

node {
    def install_root="${WORKSPACE}/install"
    stage('Set-Up Workspace') {
        deleteDir()
        checkout scm
    }
    stage('Check Code Formatting'){
        sh """
        set +x
        source /etc/profile
        module load llvm
        wget https://gist.githubusercontent.com/keipertk/c9ba20bfd92ce63a965c9c718abe9f5b/raw/1111f97432091d37d800cdef474bd0de778c1d82/.clang-format -O .clang-format
        find . -type f -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp | xargs clang-format -style=file -i -fallback-style=none
        rm .clang-format
        git diff >clang_format.patch
        if [ -s clang_format.patch ]
        then
        gem install gist
        echo '##########################################################'
        echo 'Code Formatting Check Failed!'
        echo 'Please "git apply" the Following Patch File:'
        ~/bin/gist -p clang_format.patch
        echo '##########################################################'
        exit 1
        fi
        """
    }
    stage('Build Dependencies') {
        for(int i=0; i<depends.size(); i++) {
            dir("${depends[i]}"){
                git credentialsId:'422b0eed-700d-444d-961c-1e58cc75cda2',
                    url:"https://github.com/NWChemEx-Project/${depends[i]}.git",
                    branch: 'master'
                compile_repo("${depends[i]}", "${install_root}", "True")
            }
        }
    }
    stage('Build Repo') {
        compile_repo("${repo_name}", "${install_root}", "False")
    }
    stage('Test Repo') {
        sh """
        set +x
        source /etc/profile
        module load cmake
        cd build && ctest
        """
    }
}
