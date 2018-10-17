def repoName= "Utilities"
def commonModules = "cmake llvm "
def buildModuleMatrix = [
    		   "GCC":(commonModules + "gcc/7.1.0")
		  ]
def cmakeCommandMatrix = [
    		   "GCC":"-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++"
		   ]

def credentialsID = "422b0eed-700d-444d-961c-1e58cc75cda2"

/************************************************
 ************************************************
    Shouldn't need to modify anything below...
 ************************************************    
 ************************************************/

def buildTypeList=buildModuleMatrix.keySet() as String[]		  
def nwxJenkins

node{
for (int i=0; i<buildTypeList.size(); i++){

    def buildType = "${buildTypeList[i]}"
    def cmakeCommand = "${cmakeCommandMatrix[buildType]}"

    stage("${buildType}: Set-Up Workspace"){
        deleteDir()
        checkout scm
    }

    stage('${buildType}: Import Jenkins Commands'){
        sh "wget https://raw.githubusercontent.com/NWChemEx-Project/DeveloperTools/master/ci/Jenkins/nwxJenkins.groovy"
    	nwxJenkins=load("nwxJenkins.groovy")
    }

    stage('${buildType}: Export Module List'){
        def buildModules = "${buildModuleMatrix[buildType]}"
    nwxJenkins.exportModules(buildModules)
    }

    stage('Check Code Formatting'){
        nwxJenkins.formatCode()
    }

    stage('Get CMakePackagingProject') {
        sh """
           set +x
           source /etc/profile
           module load git cmake
           git clone https://github.com/CMakePackagingProject/CMakePackagingProject
           cd CMakePackagingProject
           cmake -H. -Bbuild -DBUILD_TESTS=OFF \
                             -DCMAKE_INSTALL_PREFIX=${WORKSPACE}/install
           cd build && cmake --build --target install
           """
    }

    stage('Build Repo'){
        nwxJenkins.compileRepo(repoName, "False", cmakeCommand)
    }

    stage('Test Repo'){
        nwxJenkins.testRepo()
    }

}
}
