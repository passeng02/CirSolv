# CirSolv
Problem Statement : For a DC resistive circuit with n nodes and b branches, given the values of resistances and voltage sources in each branch, solve for the current through each branch.

Data Structures used : 

Graph(M) - 
EleCir - To model the electric circuit
Queue(A) - for the modified BFS algorithm
Queue - to store node index temporarily
Parent - to store node’s parent index temporarily
Arrays(M/A) - 
R - Matrix having data on circuit equations
Kv - Array of voltage values
Kr - Array of resistance values
BFSa,BFSp,BFSd - Arrays to store data on the modified spanning tree
Stack(A) -
Temp - Temporary stack to generate cycles
Vector(M) - 
FuCy - To store data on fundamental cycles 
HashMap(A)
To store and access the alias nodes of the meshed tree, in O(1) time.

  NOTE : 
		M - Main data structure
		A - Auxiliary data structure
    
Algorithms used : 

modBFS - A modified BFS algorithm 
genCycle - To generate cycle from modified spanning tree data
genEq - To generate KCL and KVL equations from nodes
findDet - To find Determinants
solveDet - To solve for branch currents from the determinants

Use cases :

Simulating DC resistive circuits for analysis.

