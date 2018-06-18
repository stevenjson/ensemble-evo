from subprocess import Popen, PIPE, call
import argparse
import csv
import os

def ParserConfig(parser):
    parser.add_argument('t_path', type=str, help="Path to competing treatment.")
    parser.add_argument('--num_games', type=int, default=1, help="Number of games to play against AI")
    parser.add_argument('--gen', type=str, default="5000", help="Which generation snapshot should we use?")
    parser.add_argument('--agent_ko', type=int, default=-1, help="Which agent to knockout (default: none)")
    parser.add_argument('--agent_ko_all', type=bool, default=0, help="Should we do all agent knockouts?")
    parser.add_argument('--inst_ko', type=int, default=0, help="Which instruction to knockout (default: none)")
    parser.add_argument('--rep', type=int, default=-1, help="Focus on which replicate number? (default: all replicates)")
    parser.add_argument('--timeout', type=float, default=1.0, help="How many seconds to give Minimax AI to find a move?")

def GetReplicates(treatment, main_path):
    all_replicates = os.listdir(main_path)
    rep_list = [x for x in all_replicates if treatment in x]

    return rep_list

def GetDefaultOptions():
    default_options = {}
    default_options["M"] = 0
    default_options["CF"] = 0
    default_options["C"] = 0
    default_options["REP"] = 1
    default_options["P"] = 0
    default_options["S"] = 0
    default_options["CD"] = 0
    return default_options

def GetCommand(replicate, path, args):
    options = GetDefaultOptions()
    new_options = replicate.split("_")[:-1]
    seed = replicate.split("_")[-1]

    agent_path = path + replicate + "/pop_{}/pop_{}.pop".format(args.gen, args.gen)

    for opt in new_options:
        setting = int(opt[-1])
        setting_name = opt[:-1]
        options[setting_name] = setting

    command = "./ensemble -COMPETE 1 -REPRESENTATION {} -SELECTION_METHOD {} -MULTIVOTE {} \
    -CONFIDENCE {} -COMMUNICATION {} -PENALTY {} -COORDINATOR {} \
    -ANCESTOR_FPATH {} -TIMEOUT {}".format(options["REP"], options["S"], options["M"],
                                options["CF"], options["C"], options["P"],
                                options["CD"], agent_path, args.timeout)

    if args.agent_ko > -1:
        command += " -AGENT_KO {}".format(args.agent_ko)
    if args.inst_ko > 0:
        command += " -INST_KO {}".format(args.inst_ko)

    return command

def GetResults(raw_data):
    raw_results = raw_data.split("\n")[-2]
    results = raw_results.split()
    return [int(x) for x in results]

class ResultsTable:
    def __init__(self, args):
        self.wins = 0
        self.losses = 0
        self.ties = 0
        self.invalid = 0
        self.agg_score = 0
        self.invalid_start_player = []
        self.args = args
    
    def AddResults(self, results):
        agent_score = results[0]
        ai_score = results[1]
        invalid = results[2]
        curr_player = results[3]
        start_player = results[4]

        if invalid:
            if curr_player == 0: 
                self.losses += 1
                self.invalid += 1
                self.invalid_start_player.append(start_player)
            else:
                print("AI made an invalid move. Exiting....")
                exit(-1)
        else:
            if agent_score > ai_score: self.wins += 1
            elif agent_score < ai_score: self.losses += 1
            else: self.ties += 1
            
            self.agg_score += agent_score
    
    def Print(self):
        print("Wins: {} Losses: {} Ties: {} Invalid: {} Average Score: {} Games Played: {}".format(
            self.wins, self.losses, self.ties, self.invalid, self.agg_score / self.args.num_games, self.args.num_games
        ))
    
    def GetData(self):
        return [self.wins, self.losses, self.ties, self.invalid, self.agg_score / self.args.num_games]


def PlayGames(replicate, path, args):
    command = GetCommand(replicate, path, args)
    rep_results = ResultsTable(args)
    
    for i in range(args.num_games):
        cmd = command + " -RANDOM_SEED {}".format(i + 1)
        cmd = cmd.split()
        p = Popen(cmd, stdout=PIPE, stderr=PIPE)
        stdout, stderr = p.communicate(input=str)
        stdout = stdout.decode('ascii')
        results = GetResults(stdout)
        rep_results.AddResults(results)
    
    return rep_results

def WriteCSV(csv_writer, results, args):
    data = results.GetData()
    data = data + [args.agent_ko, args.inst_ko, args.timeout, args.num_games]
    csv_writer.writerow(data)
    pass


if __name__ == "__main__":
    GROUP_SIZE = 4
    parser = argparse.ArgumentParser()
    ParserConfig(parser)
    args = parser.parse_args()

    treatment = args.t_path.split('/')[-1]
    main_path = "/".join(args.t_path.split('/')[:-1]) + "/"

    outfile = open(treatment + "T" + str(args.timeout)+".csv", 'w')
    csv_writer = csv.writer(outfile, delimiter=',')
    csv_labels = ["Wins", "Losses", "Ties", "Invalid", "Average", "Agent_KO", "Inst_KO", "Timeout", "Games_Played"]
    csv_writer.writerow(csv_labels)

    rep_list = sorted(GetReplicates(treatment, main_path))

    if args.rep > -1:
        rep_list = [x for x in rep_list if treatment + str(args.rep) in x]

    for replicate in rep_list:
        print("Treatment: {}".format(replicate))
        if args.agent_ko_all:
            for i in range(GROUP_SIZE + 1):
                args.agent_ko = i - 1
                print("Knocking out agent", args.agent_ko)
                treatment_results = PlayGames(replicate, main_path, args)
                WriteCSV(csv_writer, treatment_results, args)
        else:
            treatment_results = PlayGames(replicate, main_path, args)
            WriteCSV(csv_writer, treatment_results, args)

    outfile.close()

