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
        wget https://gist.githubusercontent.com/keipertk/e040f26162c420cc7fe235fef586b14f/raw/3d935edcbbf4b5983c586705bd4a2d545a2e3b42/.clang-format
        find . -type f -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h' -o -name '*.hh' -o -name '*.hpp'\
        | xargs clang-format -style=file -i 
        rm .clang-format
        git diff >clang_format.patch
        if [ ! -s clang_format.patch]
        then
        rm clang_format.patch
        else
        gem install gist
        gist clang_format.patch
        return1
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
