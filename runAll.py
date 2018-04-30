import os
import time

#os.system("ls")

thread_str = "./bin/CMPsim.usetrace.64 -threads 1 -t "
url_str = "./traces/"
llc_str = " -cache UL3:1024:64:16 -LLCrepl "

# file_list = ["400.perlbench.out.trace.gz", "401.bzip2.out.trace.gz", "403.gcc.out.trace.gz",
#             "410.bwaves.out.trace.gz", "416.gamess.out.trace.gz", "429.mcf.out.trace.gz",
#             "433.milc.out.trace.gz", "434.zeusmp.out.trace.gz", "435.gromacs.out.trace.gz",
#             "436.cactusADM.out.trace.gz", "437.leslie3d.out.trace.gz", "444.namd.out.trace.gz",
#             "445.gobmk.out.trace.gz", "447.dealII.out.trace.gz", "450.soplex.out.trace.gz",
#             "453.povray.out.trace.gz", "454.calculix.out.trace.gz", "456.hmmer.out.trace.gz",
#             "458.sjeng.out.trace.gz", "459.GemsFDTD.out.trace.gz", "462.libquantum.out.trace.gz",
#             "464.h264ref.out.trace.gz", "465.tonto.out.trace.gz", "470.lbm.out.trace.gz",
#             "471.omnetpp.out.trace.gz", "473.astar.out.trace.gz", "482.sphinx3.out.trace.gz",
#             "483.xalancbmk.out.trace.gz", "999.specrand.out.trace.gz", "ls.out.trace.gz"]
file_list = ["400.perlbench.out.trace.gz", "401.bzip2.out.trace.gz"]

fw = open("result.csv", 'w')
for filename in file_list:
    words = filename.split(".")
    fileword = words[0]+words[1]
    fw.write(fileword)
    for model in range(0, 3):
        time_start = time.time()
        os_str = thread_str + url_str + filename + " -o " + fileword + str(model) + ".stats" + llc_str + str(model)
        #print(os_str)
        os.system(os_str)
        os.system("gzip -d " + fileword + str(model) + ".stats" + ".gz")
        os.system("rm -f " + fileword + str(model) + ".stats.gz")
        f = open(fileword + str(model) + ".stats",'r')
        total_num = 0
        for line in f.readlines():                             
            line = line.strip()     
            total_num += 1
            if not len(line) or line.startswith('#') or total_num < 733:       
                continue                                            
            num = line.find("Miss Rate:") 
            if num == -1:
                continue   
            strings = line[num+11:num+18]
            print("Miss Rate:" + strings)   
        os.system("rm -f " + fileword + str(model) + ".stats")  
        time_end = time.time() 
        time_delta = time_end - time_start
        fw.write("," + strings + "," + str(time_delta))
    fw.write("\n")