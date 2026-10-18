import random, subprocess
random.seed(7)
n, m, maxv = 12, 1, 40
arr = [random.randint(1, maxv) for _ in range(n)]
pts = sorted(random.sample(range(1, n+1), 4))
print("n=", n, "arr=", arr, "真实查询(a,b,c,d)=", pts)
inp = str(n) + "\n" + " ".join(map(str, arr)) + "\n" + str(m) + "\n" + " ".join(map(str, pts)) + "\n"
r = subprocess.run(["/tmp/cpp158/Code03_LargestUpMedian.bin"], input=inp.encode(), capture_output=True)
print("课程输出:", r.stdout.decode().strip())
# 暴力
a,b,c,d = pts
best = None; bestseg = None
for L in range(a, b+1):
    for R in range(c, d+1):
        seg = sorted(arr[L-1:R])
        k = (R-L+2)//2
        up = seg[k-1]
        if best is None or up > best:
            best = up; bestseg = (L, R, seg, k)
print("暴力最大上中位数:", best, "区间:", bestseg)
