def repoName= "Utilities"
def depends = ["CMakeBuild"] as String[]
def commonModules = "cmake llvm "
def buildModuleMatrix = [
    		   "GCC":(commonModules + "gcc/7.1.0"),
		   "Intel":(commonModules + "intel/17.0.5")
		  ]
def cmakeCommandMatrix = [
    		   "GCC":"-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++",
		   "Intel":"-DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc"
		   ]


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
    	nwxJenkins=load("nwxJenkins.groovy")
    }

    stage('${buildType}: Export Module List'){
        def buildModules = "${buildModuleMatrix[buildType]}"
    nwxJenkins.exportModules(buildModules)
    }

    stage('Check Code Formatting'){
        nwxJenkins.formatCode()
    }

    stage('Build Dependencies'){
        nwxJenkins.buildDependencies(depends, cmakeCommand)
    }

    stage('Build Repo'){
        nwxJenkins.compileRepo(repoName, "False", cmakeCommand)
    }

    stage('Test Repo'){
        nwxJenkins.testRepo()
    }

}
}
