def f(a, l, r):
    if r - l < 2:
        return r, l
    mid = (r + l) // 2
    if a[l] < a[mid] and a[r] < a[mid]:
        return f(a, mid, r)
    if a[r] > a[mid] and a[l] > a[mid]:
        return f(a, l, mid)
    return f(a, mid, r)


def bs(a, t, l, r, s):
    if (r + s) % len(a) - (l + s) % len(a) <= 1:
        if t == a[r]:
            return r
        if t == a[l]:
            return l
        return -1
    mid = ((r + s) % len(a) + (l + s) % len(a)) // 2 - s
    if mid < 0:
        mid = len(a) + mid
    if t == a[mid]:
        return mid
    if t < a[mid]:
        return bs(a, t, l, mid, s)
    return bs(a, t, mid, r, s)
