pipeline {
    agent any
    def build_project(project_name, do_install) {
        sh '''
        set +x
        source /etc/profile
        module load gcc/7.1.0
        module load cmake
        echo 'Building ${project_name}'
        mkdir -p root
        cd ${depend_name}
        cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=${PWD}/root\
                          -DCMAKE_PREFIX_PATH=${PWD}/root
        cd build && make
        if [ ${do_install} = "True" ];then
            make install
        fi
        '''
    }
    def build_dependency(depend_name) {
        stage('Build ${depend_name}') {
            dir(depend_name){
                git credentialsId:'422b0eed-700d-444d-961c-1e58cc75cda2',
                url: 'https://github.com/NWChemEx-Project/$depend_name.git',
                branch: 'master'
            }
            build_project(depend_name, 'True')
        }
    }

    stages {
        stage('Build') {
            build_dependency('NWChemExBase')
            build_project('UtilitiesEx', 'False')
        }
        stage('Test') {
            steps {
                echo 'Testing..'
	  	        sh'''
		        set +x
     	        source /etc/profile
	            module load cmake
	            cd build
	            ctest
	            '''
            }
        }
    }
}




