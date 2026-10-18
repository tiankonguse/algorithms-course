#!/usr/bin/env python3
"""144 课对拍：课程版 C++ vs 暴力，随机小用例。"""
import random, subprocess, sys

BIN = "/tmp/cpp144/"

def run(prog, data):
    p = subprocess.run([BIN + prog], input=data, capture_output=True, text=True, timeout=30)
    if p.returncode != 0:
        return "RC" + str(p.returncode)
    return p.stdout.strip().replace("\n", " ")

def main():
    random.seed(20261017)
    ok = mism = 0
    # ---- 题2：计算系数 ----
    for it in range(3000):
        k = random.randint(0, 8)
        a = random.randint(0, 10); b = random.randint(0, 10)
        n = random.randint(0, k); m = k - n
        inp = f"{a} {b} {k} {n} {m}\n"
        c = run("course_02", inp); v = run("bf_02", inp)
        if c == v: ok += 1
        else:
            mism += 1
            if mism <= 5: print("MISMATCH 题2", inp.strip(), "course=", c, "bf=", v)
    print(f"题2 计算系数: OK={ok} MISMATCH={mism}")
    if mism: sys.exit(1)
    # ---- 题3：组合数问题 ----
    ok = mism = 0
    for it in range(2000):
        k = random.randint(2, 8)
        t = random.randint(1, 5)
        qs = []
        for _ in range(t):
            n = random.randint(0, 40); m = random.randint(0, 40)
            qs.append(f"{n} {m}")
        inp = f"{t} {k}\n" + "\n".join(qs) + "\n"
        c = run("course_03", inp); v = run("bf_03", inp)
        if c == v: ok += 1
        else:
            mism += 1
            if mism <= 5: print("MISMATCH 题3", inp.strip().replace("\n", ";"), "course=", c, "bf=", v)
    print(f"题3 组合数问题: OK={ok} MISMATCH={mism}")
    if mism: sys.exit(1)
    # ---- 题4：分割方法数（课上版 vs ways1，LC 版 vs ways0）----
    ok = mism = 0
    for it in range(20000):
        n = random.randint(1, 9)
        arr = [random.randint(1, 15) for _ in range(n)]
        inp = f"{n}\n" + " ".join(map(str, arr)) + "\n"
        c4 = run("course_04", inp); c5 = run("course_05", inp)
        v = run("bf_04", inp)
        parts = v.split()
        if len(parts) != 2:
            mism += 1
            if mism <= 5: print("FAIL 题4", inp.strip(), "bf=", v)
            continue
        if c4 == parts[0] and c5 == parts[1]: ok += 1
        else:
            mism += 1
            if mism <= 5: print("MISMATCH 题4", inp.strip(), "course4=", c4, "course5=", c5, "bf=", v)
    print(f"题4 分割方法数: OK={ok} MISMATCH={mism}")
    sys.exit(1 if mism else 0)

if __name__ == "__main__":
    main()
