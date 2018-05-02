from subprocess import Popen, PIPE, call
import os
import itertools
import csv

def CreateProgramFiles(pop_path, gp_path):
    if not os.path.isdir(pop_path):
        print(f"Invalid path '{pop_path}'")
        exit(-1)
    
    if not os.path.isdir(gp_path): call(["mkdir", gp_path])
    
    all_run_dir = os.listdir(pop_path)
    method_dir = []

    method_dir.append([x for x in all_run_dir if "REP0_S0_GEN2000" in x])
    method_dir.append([x for x in all_run_dir if "REP0_S1_GEN2000" in x])
    method_dir.append([x for x in all_run_dir if "REP1_S0_GEN2000" in x])
    method_dir.append([x for x in all_run_dir if "REP1_S1_GEN2000" in x])
    method_dir.append([x for x in all_run_dir if "REP1_S0_GEN1000" in x])
    method_dir.append([x for x in all_run_dir if "REP1_S1_GEN1000" in x])
    method_dir.append([x for x in all_run_dir if "REP2_S0_GEN2000" in x])
    method_dir.append([x for x in all_run_dir if "REP2_S1_GEN2000" in x])

    for m in range(0, len(method_dir)):
        m_dir = "".join(method_dir[m][0].rsplit("_", 1)[0]) +"/"
        pop_type = m_dir.split("GEN")[1][:-1]
        method_type = m_dir.split("_")
        if not os.path.isdir(gp_path + m_dir): call(["mkdir", gp_path + m_dir])

        for dir_name in method_dir[m]:
            genome = []
            in_file = open(pop_path+dir_name+f"/pop_{pop_type}/pop_{pop_type}.pop", 'r')
            out_file = open(gp_path + m_dir + dir_name + ".gp", 'w')
            all_prog = in_file.read()
            if method_type[0] == "REP0":
                genome = all_prog.split("$", 1)[0]
                out_file.write(genome)
            elif method_type[0] == "REP1" or method_type[0] == "REP2":
                genome = all_prog.split("$", 5)[:-1]
                for prog in genome: 
                    out_file.write(prog + "$\n")

            in_file.close()
            out_file.close()

if __name__ == '__main__':
    treatments = ["REP0_S0_GEN2000", "REP0_S1_GEN2000", "REP1_S0_GEN2000", "REP1_S1_GEN2000",
                    "REP1_S0_GEN1000", "REP1_S1_GEN1000", "REP2_S0_GEN2000", "REP2_S1_GEN2000"]
    
    data_labels = ["Matchup", "P1_Wins", "P2_Wins", "Ties", "P1_Invalid_Moves", "P2_Invalid_Moves", "P2_First"]

    pop_path = '../Evolved_Agents/'
    gp_path = './programs/'

    CreateProgramFiles(pop_path, gp_path)

    csv_out = open("compete_results.csv", "w")
    csv_writer = csv.writer(csv_out, delimiter=',')
    csv_writer.writerow(data_labels)

    combinations = itertools.combinations(treatments, 2)
    combo_num = 1
    total_data = {}
    for t in treatments:
        total_data[t] = [0, 0, 0]

    for combo in combinations:
    
        compete_type = 0
        path_1 = combo[0]
        path_2 = combo[1]
        data = [0, 0, 0, 0, 0, 0]
        seed = 1234

        if path_1.split('_')[0] == "REP0":
            if path_2.split('_')[0] == "REP0":
                compete_type = 0
            else:
                compete_type = 1
        else:
            compete_type = 2 
        
        path_1_files = os.listdir(gp_path + path_1)
        path_2_files = os.listdir(gp_path + path_2)
        path_1_files = sorted(path_1_files)
        path_2_files = sorted(path_2_files)
        count = 0
        for file_1 in path_1_files:
            for file_2 in path_2_files:
                p1 = gp_path + path_1 + "/" + file_1
                p2 = gp_path + path_2 + "/" + file_2
                cmd = './ensemble -RANDOM_SEED {} -COMPETE 1 -COMPETE_TYPE {} -COMPETE_FPATH_1 {} -COMPETE_FPATH_2 {}'.format(seed, compete_type, p1, p2)
                seed += 1
                cmd = cmd.split()
                p = Popen(cmd, stdout=PIPE, stderr=PIPE)
                stdout, stderr = p.communicate(input=str)
                output = str(stdout).split('!')[1].replace("\\n'", "").replace("\\n", "")
                output = output.split(" ")
                # if int(output[2]) == 1:
                #     print("Start Player:", output[4]," End Player:", output[3], "P1:", file_1, "P2:", file_2)
                count += 1
                data[5] += int(output[4])
                if int(output[2]) == 1:
                    if int(output[3]) == 0: data[3] += 1
                    elif int(output[3]) == 1: data[4] += 1
                    else: print("ER", int(output[3]) == 0)
                if int(output[0]) > int(output[1]): 
                    data[0] += 1
                elif int(output[0]) < int(output[1]): 
                    data[1] += 1
                elif int(output[0]) == int(output[1]): 
                    data[2] += 1
                else:
                    print("ERROR")
                        
        print("({}/{})".format(combo_num, "28"), path_1 + " Wins:",data[0], path_2 + " Wins:", data[1], " Ties:", data[2], end=" ")
        print(path_1 + " Inv.:", data[3], path_2 + " Inv.:", data[4], " P2 Went First:", data[5])
        
        combo_num += 1
        csv_writer.writerow([path_1 + ";" + path_2] + data)
        total_data[path_1][0] += data[0]
        total_data[path_1][1] += data[2]
        total_data[path_1][2] += data[3]
        total_data[path_2][0] += data[1]
        total_data[path_2][1] += data[2]
        total_data[path_2][2] += data[4]

    print("\nResults\n-------\n")
    for key in total_data.keys():
        print(key, "Wins:", total_data[key][0], "Ties:", total_data[key][1], "Invalid:", total_data[key][2])

    csv_out.close()
