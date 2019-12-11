n = int(input())
a = list(map(int, input().split()))
c = int(input())
tbl = [[0] * c for _ in range(n + 1)]
for i in range(1, n + 1):
    for j in range(1, c):
        tbl[i][j] = tbl[i - 1][j]
        if j >= a[i - 1]:
            tbl[i][j] = max(
                tbl[i][j],
                tbl[i - 1][j - a[i - 1]] + a[i - 1]
            )
print(tbl[-1][-1])
i = n
j = c - 1
while i:
    if tbl[i][j] != tbl[i - 1][j]:
        j -= a[i - 1]
        print(i, end=' ')
    i -= 1
