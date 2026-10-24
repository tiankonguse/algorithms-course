import random, subprocess, sys
COURSE1 = "/tmp/cpp199/Code01_DirectedRoads2.run"
COURSE2 = "/tmp/cpp199/Code02_NumberOfSimplePaths2.run"
COURSE3 = "/tmp/cpp199/Code03_Knight2.run"
COURSE4 = "/tmp/cpp199/Code04_RingRoad2.run"
BR1 = "/tmp/cpp199/brute1.run"
BR2 = "/tmp/cpp199/brute2.run"
BR3 = "/tmp/cpp199/brute3.run"
BR4 = "/tmp/cpp199/brute4.run"
G = "/tmp/cpp199/gen199.py"

def run(cmd, inp=None):
    return subprocess.run(cmd, input=inp, capture_output=True, text=True).stdout.strip()

ok1 = ok2 = ok3 = ok4 = 0
fails = []
for seed in range(1, 13):
    inp1 = run(["python3", G, "t1", str(seed), "8"])
    c = run([COURSE1], inp1.split("\n", 1)[1])
    b = run([BR1], inp1)
    if c == b: ok1 += 1
    else: fails.append(f"题1 seed{seed}: 课程{c} 暴力{b}")

    inp2 = run(["python3", G, "t2", str(seed), "8"])
    c = run([COURSE2], inp2.split("\n", 1)[1])
    b = run([BR2], inp2)
    if c == b: ok2 += 1
    else: fails.append(f"题2 seed{seed}: 课程{c} 暴力{b}")

    inp3 = run(["python3", G, "t3", str(seed), "8"])
    c = run([COURSE3], inp3.split("\n", 1)[1])
    b = run([BR3], inp3)
    if c == b: ok3 += 1
    else: fails.append(f"题3 seed{seed}: 课程{c} 暴力{b}")

    inp4 = run(["python3", G, "t4", str(seed), "8"])
    c = run([COURSE4], inp4.split("\n", 1)[1])
    b = run([BR4], inp4)
    if c == b: ok4 += 1
    else: fails.append(f"题4 seed{seed}: 课程{c} 暴力{b}")

print(f"== 题1: {ok1}/12  题2: {ok2}/12  题3: {ok3}/12  题4: {ok4}/12 ==")
for f in fails: print(f)
