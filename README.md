# IPA

## Instruction

This is an implementation of "Simplification of large-scale triangle meshes using geometric and radiometric information", which is done within the frame Interdisciplinary Project Work of master program Geomatics at ETH. 

This file contains instructions on how to set up your environment to work with the source code for this project.


## Original Work

The code is built upon a great mesh refinement work from Dr. Mathias Rothermel et al. The following link contains a thorough description of their work:

https://bitbucket.org/mathiaro/meshref/overview

It is very recommended to go through the instruction there, which includes dependencies, compilation and even the data. Please note that you have to adapt the file "Makefile.rules" for your own dependency environment.  


## Modification
If the previous step is done, you should have a full functionality of the original work. For this project, only parts of the code from the original work will be needed. Here are the modifications that should be done:

1) Copy all 5 folders within the folder "Mesh simplification code" to your path and
   replace them with existing ones if asked.

2) Copy all files within the folder "Mesh simplification data" to the file "data" in your
   path

Please note that the original mesh data is "Mesh_cut.ply" in the folder "data". For a better performance of this project, it is recommended to densify the mesh with the program MeshLab into a level of more than 1M triangles.

## Running
Now the code is ready for running, if the previous steps are done smoothly. You can choose any IDEs you like to run it. Here it shows how to do it with Eclipse as an example:

1) Create a new Workspace and initialise a Makefile project. 
2) Add the following folders to the Project Explorer:
   
   * IPA_Simplify
   * LikelihoodImage
   * MeshUtil
   * Ori

3) Compile the above 4 folders with the given makefiles individually.
4) Run the program

The main script for this project is the file "IPA_Simplify.cpp". It shows the main structure of this project and includes all parameters that can be adapted for different settings. 


## Questions
The algorithm of this project is described in the report "ipa_JingtongLi.pdf". If you have any questions, feel free to contact with lijing@ethz.ch

