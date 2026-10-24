#!/usr/bin/env python3
"""dui196.py：8 题 × N 种子对拍。课程程序 *.run vs brute196。
比较策略：题1/2/3/4/8 比对可行性 + 验证课程方案合法性；题5 数直比；题6/7 判词直比。
"""
import subprocess, sys, os

CPP = '/tmp/cpp196'
SEEDS = range(1, 13)
COURSE = ['Code01_2SAT2', 'Code02_Committee2', 'Code03_Astronauts2', 'Code04_Constraints2',
          'Code05_Conspiracy2', 'Code06_Riddle2', 'Code07_Coprime2', 'Code08_RadioStations2']
BRUTE = '/tmp/cpp196/brute196.run'

def run(cmd, inp):
    p = subprocess.run(cmd, input=inp.encode(), capture_output=True)
    return p.stdout.decode().strip()

def norm_lines(s):
    return [l.strip() for l in s.strip().split('\n') if l.strip()]

def verify_solution(no, inp, out):
    """对课程输出做独立合法性验证（可行性 + 方案合法）。返回 True/False + 说明。"""
    lines = norm_lines(out)
    nums = inp.strip().split('\n')
    if no == 1:
        # POSSIBLE / IMPOSSIBLE + 赋值
        if lines[0] == 'IMPOSSIBLE': return True, 'IMPOSSIBLE'
        val = [int(x) for x in lines[1].split()]
        n = int(nums[0].split()[0])
        if len(val) != n: return False, f'赋值长度 {len(val)} ≠ n {n}'
        for l in nums[1:]:
            x1, v1, x2, v2 = map(int, l.split())
            b1 = val[x1 - 1] == v1; b2 = val[x2 - 1] == v2
            if not (b1 or b2): return False, f'子句不满足: {l}'
        return True, 'POSSIBLE'
    if no == 2:
        if lines[0] == 'NIE': return True, 'NIE'
        sel = [int(x) for x in lines]
        n = int(nums[0].split()[0])
        if len(sel) != n: return False, f'人数 {len(sel)} ≠ n {n}'
        for i in range(1, n + 1):
            if not (sel.count(2 * i - 1) == 1 or sel.count(2 * i) == 1):
                return False, f'团体 {i} 选择异常: {sel}'
        for l in nums[1:]:
            u, v = map(int, l.split())
            if u in sel and v in sel: return False, f'讨厌同席: {l}'
        return True, 'OK'
    if no == 3:
        if 'No solution.' in out: return True, 'No solution.'
        tasks = lines
        n = int(nums[0].split()[0])
        if len(tasks) != n: return False, f'任务数 {len(tasks)} ≠ n {n}'
        ages = list(map(int, nums[1].split()))
        s = sum(ages)
        for i in range(1, n + 1):
            older = ages[i - 1] * n >= s
            t = tasks[i - 1]
            if t == 'A' and not older: return False, f'{i} 年龄不够做 A'
            if t == 'B' and older: return False, f'{i} 年龄够却做 B'
        for l in nums[2:]:
            parts = l.split()
            if len(parts) < 2: continue
            x, y = int(parts[0]), int(parts[1])
            if x == 0 and y == 0: continue
            if tasks[x - 1] == tasks[y - 1]: return False, f'讨厌同任务: {l}'
        return True, 'OK'
    if no == 4:
        it_in = iter(nums)
        t = int(next(it_in))
        out_lines = iter(lines)
        for _ in range(t):
            n, m, k = map(int, next(it_in).split())
            cons = []
            for _c in range(m):
                parts = list(map(int, next(it_in).split()))
                cons.append(parts)
            out_line = next(out_lines)
            if out_line == '-1': continue
            arr = [int(x) for x in out_line.split()]
            if len(arr) != n: return False, f'arr 长 {len(arr)} ≠ n {n}'
            for i in range(n - 1):
                if arr[i] > arr[i + 1]: return False, f'非降破坏: {arr}'
            for v in arr:
                if v < 1 or v > k: return False, f'值越界: {v}'
            for parts in cons:
                if parts[0] == 1:
                    if arr[parts[1] - 1] == parts[2]: return False, f'不等于破坏: {parts}'
                elif parts[0] == 2:
                    if arr[parts[1] - 1] + arr[parts[2] - 1] > parts[3]: return False, f'≤破坏: {parts}'
                else:
                    if arr[parts[1] - 1] + arr[parts[2] - 1] < parts[3]: return False, f'≥破坏: {parts}'
        return True, 'OK'
    if no == 8:
        if lines[0] == '-1': return True, '-1'
        cnt, ax = map(int, lines[0].split())
        pick = [int(x) for x in lines[1].split()]
        if len(pick) != cnt: return False, f'数量 {len(pick)} ≠ {cnt}'
        it = iter(nums)
        n, p, f, m = map(int, next(it).split())
        lr = {}
        req = []
        for _ in range(n):
            u, v = map(int, next(it).split()); req.append((u, v))
        for i in range(1, p + 1):
            l, r = map(int, next(it).split()); lr[i] = (l, r)
        ban = []
        for _ in range(m):
            u, v = map(int, next(it).split()); ban.append((u, v))
        if ax < 1 or ax > f: return False, f'ansx 越界: {ax}'
        for i in pick:
            l, r = lr[i]
            if not (l <= ax <= r): return False, f'电台 {i} 不含 {ax}'
        for u, v in req:
            if u not in pick and v not in pick: return False, f'要求未满足: {u} {v}'
        for u, v in ban:
            if u in pick and v in pick: return False, f'限制被破: {u} {v}'
        return True, 'OK'
    return False, '未知题号'

def compare(no, brute_out, course_out):
    if no in (5, 6, 7):
        return brute_out.strip() == course_out.strip(), f'暴力={brute_out!r} 课程={course_out!r}'
    if no in (1, 2, 3, 4, 8):
        # 可行性比较：POSSIBLE/IMPOSSIBLE、NIE/方案、No solution./方案、-1/方案
        def feasible(s, no):
            if no == 1: return 'POSSIBLE' in s
            if no == 2: return s != 'NIE'
            if no == 3: return 'No solution.' not in s
            if no in (4, 8): return not (s.startswith('-1'))
        bf, cf = feasible(brute_out, no), feasible(course_out, no)
        if bf != cf:
            return False, f'可行性不一致 暴力={brute_out[:60]!r} 课程={course_out[:60]!r}'
        return True, f'可行性一致'
    return False, '?'

def main():
    only = sys.argv[1] if len(sys.argv) > 1 else None
    fails = 0; total = 0
    for no in range(1, 9):
        if only and int(only) != no: continue
        c = COURSE[no - 1]
        for seed in SEEDS:
            total += 1
            inp = subprocess.run(['python3', '/tmp/cpp196/gen196.py', str(no), str(seed)],
                                 capture_output=True).stdout.decode()
            bo = run([BRUTE, str(no)], inp)
            co = run([f'{CPP}/{c}.run'], inp)
            ok, msg = compare(no, bo, co)
            if ok:
                # 再验证课程方案合法性（对方案类）
                if no in (1, 2, 3, 4, 8):
                    vok, vmsg = verify_solution(no, inp, co)
                    if not vok:
                        fails += 1
                        print(f'题{no} seed{seed} 方案非法: {vmsg}')
                        open(f'/tmp/cpp196/fail_p{no}_s{seed}.in', 'w').write(inp)
                        print('  输入已存 fail_p%d_s%d.in' % (no, seed))
                        continue
                print(f'题{no} seed{seed}: OK')
            else:
                fails += 1
                print(f'题{no} seed{seed}: MISMATCH {msg}')
                open(f'/tmp/cpp196/fail_p{no}_s{seed}.in', 'w').write(inp)
                print('  输入已存 fail_p%d_s%d.in' % (no, seed))
    print(f'== {total - fails}/{total} 组通过 ==')
    sys.exit(1 if fails else 0)

if __name__ == '__main__':
    main()
