# DS_final_project
2019_DS_final_project

This is a project to solve k-core problem and maximum clique problem.
See the reference here (http://insilab.org/articles/match2007.pdf) to know more about finding maximum clique using branch and bound and coloring algorithm.

Notice that there is still some room for improvement due to lack of RAM.

# compile
g++11 standard 
see Makefile to get more detail.

# input
open_testcase_ds.txt contians multiple lines of pairs, each pair represents each edge in the graph.

# output
Output two files, kcore.txt and clique.txt, the first file should contain all the edges of which coreness is greater than the desire value and the other file should contain the nodes of maximum clique with ascending order of index.

