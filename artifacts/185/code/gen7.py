import random, sys
random.seed(int(sys.argv[1]))
n = random.randint(2, 10)
print(n)
for v in range(2, n+1):
    print(random.randint(1, v-1), v)
m = random.randint(5, 15)
print(m)
for _ in range(m):
    if random.random() < 0.6:
        print('C', random.randint(1, n))
    else:
        print('G')
