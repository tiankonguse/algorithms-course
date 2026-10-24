import subprocess
C5="/tmp/cpp199/Code05_DiameterMinimize2.run"
C6="/tmp/cpp199/Code06_Island2.run"
C7="/tmp/cpp199/Code07_ShortestDistance_cpp_2.run"
C71="/tmp/cpp199/Code07_cpp1.run"
C8="/tmp/cpp199/Code08_Journey2.run"
B5="/tmp/cpp199/brute5.run"
B6="/tmp/cpp199/brute6.run"
B7="/tmp/cpp199/brute7.run"
B8="/tmp/cpp199/brute8.run"
G="/tmp/cpp199/gen199.py"
def run(cmd, inp=None):
    return subprocess.run(cmd, input=inp, capture_output=True, text=True).stdout.strip()
ok5=ok6=ok7=ok8=ok71=0
fails=[]
for seed in range(1,13):
    inp5 = run(["python3",G,"t5",str(seed),"9"])
    c=run([C5],inp5.split("\n",1)[1]); b=run([B5],inp5)
    if c==b: ok5+=1
    else: fails.append(f"题5 seed{seed}: 课程{c} 暴力{b}")

    inp6 = run(["python3",G,"t6",str(seed),"8"])
    c=run([C6],inp6.split("\n",1)[1]); b=run([B6],inp6)
    if c==b: ok6+=1
    else: fails.append(f"题6 seed{seed}: 课程{c} 暴力{b}")

    inp7 = run(["python3",G,"t7",str(seed),"7"])
    c=run([C7],inp7.split("\n",1)[1]); b=run([B7],inp7)
    if c==b: ok7+=1
    else: fails.append(f"题7好想解 seed{seed}: 课程{c} 暴力{b}")
    c1=run([C71],inp7.split("\n",1)[1])
    if c1==b: ok71+=1
    else: fails.append(f"题7巧解 seed{seed}: 课程{c1} 暴力{b}")

    inp8 = run(["python3",G,"t8",str(seed),"8"])
    c=run([C8],inp8.split("\n",1)[1]); b=run([B8],inp8)
    if c==b: ok8+=1
    else: fails.append(f"题8 seed{seed}: 课程{c} 暴力{b}")
print(f"== 题5:{ok5}/12 题6:{ok6}/12 题7好想:{ok7}/12 题7巧解:{ok71}/12 题8:{ok8}/12 ==")
for f in fails: print(f)
