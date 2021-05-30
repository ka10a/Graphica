# your code goes here
# your code goes here
def func1(a, b, c, d):
    return [a, b, c, d, a, d, c, b]

def func2(x, y, z, a, b, c):
    t = func1(y, z, b, c)
    tmp = []      
    for i in range(4):
        tmp.append((x, t[2 * i], t[2 * i + 1]))
        if i == 2:
            tmp.append(tmp[0])
            tmp.append(tmp[1])
    return tmp
        
def switch1(a, b, c):
    return b, c, a

def switch2(a, b, c):
    return c, a, b

def cube_to_triangle(x, y, z, a, b, c):
    if x == a:
        tmp = func2(x, y, z, a, b, c)
        for elem in tmp:
            yield elem
    if y == b:
        x, y, z = switch1(x, y, z)
        a, b, c = switch1(a, b, c)
        tmp = func2(x, y, z, a, b, c)
        for elem in tmp:
            yield switch2(elem[0], elem[1], elem[2])
    if z == c:
        x, y, z = switch2(x, y, z)
        a, b, c = switch2(a, b, c)
        tmp = func2(x, y, z, a, b, c)
        for elem in tmp:
            yield switch1(elem[0], elem[1], elem[2])
    
def cube_from_2p(x, y, z, a, b, c):
    t = [0] * 7
    t[1] = [_ for _ in cube_to_triangle(x, y, z, x, b, c)]
    t[2] = [_ for _ in cube_to_triangle(x, y, z, a, y, c)]
    t[3] = [_ for _ in cube_to_triangle(x, y, z, a, b, z)]
    t[4] = [_ for _ in cube_to_triangle(x, y, c, a, b, c)]
    t[5] = [_ for _ in cube_to_triangle(x, b, z, a, b, c)]
    t[6] = [_ for _ in cube_to_triangle(a, y, z, a, b, c)]
    for elem in t[1:]:
        for q in elem:
            yield q
            
arr = [
    [0, 0, 0, 8, 3, 3],
    [7, 0, 3, 11, 3, 6],
    [7, 0, 6, 8, 1, 7],
    [7, 2, 6, 8, 3, 7],
    [-1, 1, 2, 0, 2, 3],
    [7, 0, -1, 8, 1, 0],
    [7, 2, -1, 8, 3, 0],
    [0, 2, -1, 1, 3, 0],
    [0, 0, -1, 1, 1, 0]
]

for elem in arr:
    for e1 in cube_from_2p(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5]):
        print("{}.0f,{}.0f,{}.0f,".format(e1[0], e1[1], e1[2]))


 
