from subprocess import Popen, PIPE, call
import os

def CreateProgramFiles(pop_path, gp_path):
    if not os.path.isdir(pop_path):
        print(f"Invalid path '{pop_path}'")
        exit(-1)
    
    if not os.path.isdir(gp_path): call(["mkdir", gp_path])
    
    all_run_dir = os.listdir(pop_path)
    method_dir = []

    method_dir.append([x for x in all_run_dir if "REP0_S0_GEN2000" in x])
    method_dir.append([x for x in all_run_dir if "REP0_S1_GEN2000" in x])
    #method_dir.append([x for x in all_run_dir if "REP1_S0_GEN2000" in x])
    #method_dir.append([x for x in all_run_dir if "REP1_S1_GEN2000" in x])

    for m in range(0, len(method_dir)):
        m_dir = "".join(method_dir[m][0].rsplit("_", 1)[0]) +"/"
        pop_type = m_dir.split("GEN")[1][:-1]
        if not os.path.isdir(gp_path + m_dir): call(["mkdir", gp_path + m_dir])

        for dir_name in method_dir[m]:
            in_file = open(pop_path+dir_name+f"/pop_{pop_type}/pop_{pop_type}.pop", 'r')
            out_file = open(gp_path + m_dir + dir_name + ".gp", 'w')
            all_prog = in_file.read()
            genome = all_prog.split("$", 1)[0]
            out_file.write(genome)
            in_file.close()
            out_file.close()

            



if __name__ == '__main__':
    compete_type = 0
    pop_path = '../Ensemble_Results/'
    gp_path = './programs/'

    CreateProgramFiles(pop_path, gp_path)

    cmd = './ensemble -COMPETE 1 -COMPETE_TYPE 0 -COMPETE_FPATH_1 ./ancestor.gp -COMPETE_FPATH_2 ./ancestor.gp'

    methods_path = os.listdir(gp_path)
    data = [0,0,0,0]
    path_1 = methods_path[0]
    path_2 = methods_path[1]
    
    path_1_files = os.listdir(gp_path + path_1)
    path_2_files = os.listdir(gp_path + path_2)

    for file_1 in path_1_files:
        for file_2 in path_2_files:
            p1 = gp_path + path_1 + "/" + file_1
            p2 = gp_path + path_2 + "/" + file_2
            print(p1, p2)
            cmd = './ensemble -RANDOM_SEED 1 -COMPETE 1 -COMPETE_TYPE {} -COMPETE_FPATH_1 {} -COMPETE_FPATH_2 {}'.format(compete_type,p1, p2)
            cmd = cmd.split()
            p = Popen(cmd, stdout=PIPE, stderr=PIPE)
            stdout, stderr = p.communicate(input=str)
            output = str(stdout).split('!')[1].replace("\\n'", "")
            output = output.split(" ")
            if int(output[-1]) == 1: data[-1] += 1
            elif int(output[1]) > int(output[3]): data[0] += 1
            elif int(output[1]) < int(output[3]): data[1] += 1
            elif int(output[1]) == int(output[3]): data[3] += 1
                    
    print(data)
