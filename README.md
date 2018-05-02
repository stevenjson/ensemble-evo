CSE 812 Final Project - Evolution of Distributed Othello Players
==================================================================

Authors: Steven Jorgensen and Olive Miller

NOTE:    A unix environment is required to run this project. Please make sure you have G++ 5.4 or above installed.
         Please make sure you have python 3.6 or above installed.

Directory Structure
--------------------

Empirical - Software library needed for the project, developed by Dr. Charles Ofria and collaborators

ensemble-evo - The main directory for our project code

Evolved_Agents - All of the data from the evolution runs. Individual runs have their own directory labeled as follows: REPx_Sy_GENz_seed where:
                x = Representation used (e.g. REP0), y = selection method used (e.g. S1), z = number of generation run for (e.g. GEN2000), and seed = the random seed used

TransitiveGroupIndividuals - The selected evolved individual agents that are used to seed the group evolution of Transitive Group Evolution


How to Run
------------

For generating the results from the competition you will need to do the following:

1. cd into the ensemble-evo directory
2. run the following command: make
3. once the project is compiled, run: python3 compete.py

The agents will begin to compete, and when it finishes, there will be a file called "compete_results.csv" created containing the results.


For running the code to evolve the agents, you will need to do the following:

1. cd into the ensemble-evo directory
2. run the following command: make
3. the command to start the evolution is as follows:
    ./ensemble -RANDOM_SEED [seed] -REPRESENTATION [rep] -SELECTION_METHOD [s] -ANCESTOR_FPATH [anc_fpath] -GENERATIONS [gens]

    where:
    seed = the desired random seed

    rep = the representation to use. 0 = individual, 1 = group, 2 = group with communication

    s = selection method to use. 0 = tournament, 1 = lexicase

    anc_fpath = file path to ancestor program. "ancestor.gp" for individual runs, "ancestor_group.gp" for ensemble runs

    gens = number of generations to evolve for, default is 2000

4. Once the code is done running, the following files will be generated:

    fitness.csv - the fitness results for the run

    best_phenotype.csv - the scores from the best agent against each of the heuristic functions

    pop_0/ - a directory which holds a snapshot of the genomes of the starting population

    pop_1000/ - a directory which holds a snapshot of the population genomes after 1000 generations NOTE: File will not be generated if it evolves for less generations than 1000

    pop_2000/ - same as above, but after 2000 generations NOTE: File will not be generated if it evolves for less generations than 2000