import random, sys
seed = int(sys.argv[1]); random.seed(seed)
n = random.randint(1, 10)
m = random.randint(1, n)
perm = list(range(1, n+1)); random.shuffle(perm)
print(n, m)
print(' '.join(map(str, perm)))
del_list = perm[:]
random.shuffle(del_list)
print(' '.join(map(str, del_list[:m])))
