#
# If you want to plot the output, you can do the following:
#
# python kadai03.py | gnuplot -e " plot '-'"
#

def rand_val(minval = 0.0, maxval = 1.0):
    x = 1
    a = 1664525
    c = 1013904223
    m = 4294967296
    while True:
        xnext = (a * x + c) % m
        yield minval + (maxval - minval) * xnext / m
        x = xnext

def random_walk(x0, p):
    if p > 1 or p < 0:
        raise ValueError('Variable "p" needs to be in the range of [0, 1]')
    else:
        r = rand_val()
        xn = x0
        while True:
            if next(r) <= p:
                xn += 1
            else:
                xn -= 1
            yield xn

rw = random_walk(0, 0.5)
for n in range(1000):
    print(str(n) + " " + str(next(rw)))
