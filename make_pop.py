import sys
import os

out_path = "./new_pop/"
in_path = "../Ensemble_Results/"

all_dir_list = os.listdir(in_path)

selection = 0

dir_list = [x for x in all_dir_list if "S{}_GEN1000".format(selection) in x]

out_file = open("make_pop.py", 'r')
count = 0

for i in range(0, len(dir_list)):
    if i % 5 == 0:
        out_file.close()
        print("Opening File {}".format(count))
        out_file = open(out_path + "S{}_{}.gp".format(selection, count), 'w')
        count += 1
    
    in_file = open(in_path + dir_list[i] + "/pop_1000/pop_1000.pop", 'r')
    data = in_file.read()
    genome = data.split('$', 1)[0]
    out_file.write(genome + "$\n")

out_file.close()
