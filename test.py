def bs(a, l, r):
    if r - l < 2:
        print(l, r)
        if l == a[l] and r != a[r]:
            return r
        if l == 0:
            return 0
        return -1
    mid = (r + l) // 2
    if mid != a[mid]:
        return bs(a, l, mid)
    return bs(a, mid, r)
