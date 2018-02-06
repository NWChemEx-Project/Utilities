node {
    stage('Build dependencies') {
        def dependencies = [ "NWChemExBase" ]
        for(int i=0; i<dependencies.size(); i++) {
            def depend_name = dependencies[i]
            dir(depend_name){
                git credentialsId:'422b0eed-700d-444d-961c-1e58cc75cda2',
                url:'https://github.com/NWChemEx-Project/" + depend_name '.git',
                branch: 'master'
            }
            sh '''
            set +x
            source /etc/profile
            module load gcc/7.1.0
            module load cmake
            echo 'Building ${depend_name}'
            mkdir -p root
            cd ${depend_name}
            cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=${PWD}/root\
                              -DCMAKE_PREFIX_PATH=${PWD}/root
            cd build && make install
            '''
        }
    }
    stage('Build UtilitiesEx') {
        sh '''
        set +x
        source /etc/profile
        module load gcc/7.1.0
        module load cmake
        cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=${PWD}/root\
                          -DCMAKE_PREFIX_PATH=${PWD}/root
        cd build && make
        '''
    }
    stage('Test') {
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




