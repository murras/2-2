import os
import sys
import subprocess
import copy

traces = [ '../mtraces/'+ a for a in os.listdir('../mtraces') ]
len_traces = len(traces)

pass_count = 0
fail_count = 0

def pagetable_metric( nums , arg_list , trace_list ):
    global pass_count
    global fail_count

    if nums == 0 or trace_list == []:
        base_dir = os.path.dirname(os.path.realpath(__file__))
        cmd_list = [ [base_dir + '\\memsim.exe'] , [base_dir + '/memsim'] ]

        results = []
        _ = [ cmd_list[i].extend(arg_list) for i in range(0, 2) ]

        for cmd in cmd_list:
            results.append(subprocess.check_output(cmd))

        if results[0] == results[1]:
            print(" ".join(cmd[3:]) + " : PASS")
            pass_count += 1
        else:
            print(" ".join(cmd[3:]) + " : FAIL")
            fail_count += 1
            
    else:
        for i in range(0, len(trace_list)):
            new_arg_list = copy.deepcopy(arg_list) # Amazing Python
            new_arg_list.append(trace_list[i])
            pagetable_metric(nums-1, new_arg_list, trace_list)

                             
print("FIRST PAGE SIZE BIT : " + sys.argv[1])
print("PHYSICAL MEMORY SIZE : " + sys.argv[2])

pagetable_metric(int(sys.argv[3]), [sys.argv[1], sys.argv[2]], traces)

print(str(pass_count) + " cases PASSED")
print(str(fail_count) + " cases FAILED")