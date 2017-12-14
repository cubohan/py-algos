def base(x):
    count = 1
    x /= 10
    while (x>0):
        x /= 10
        count += 1
    return count

def katsura(x, y, b1, b2):
    # import ipdb
    # ipdb.set_trace()
    if b1 <= 1 or b2 <= 1:
        return x*y

    midx = b1/2
    midy = b2/2

    x1 = x / 10**midx
    x2 = x % 10**midx

    y1 = y / 10**midy
    y2 = y % 10**midy

    # basex = midx if (x1+x2) / 10**mid == 0 else mid + 1
    # basey = midy if (y1+y2) / 10**mid == 0 else mid + 1
    p1 = 10**(midx+midy) * katsura(x1, y1, b1-midx, b2-midy)
    p2 = 10**(midx) * (katsura(x1, y2, b1-midx, midy))
    p3 = 10**(midy) * katsura(x2, y1, midx, b2-midy)
    p4 = katsura(x2, y2, midx, midy)
    return p1 + p2 + p3 + p4

b = 64
b = 64

print katsura(3141592653589793238462643383279502884197169399375105820974944592, 2718281828459045235360287471352662497757247093699959574966967627, b, b)

#print katsura(101, 10, 2, 2)