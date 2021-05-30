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


def half_of_cube_to_triangle(x, y, z, a, b, c):
    points = [(x, y, z), (a, y, z), (x, b, z), (x, y, c)]
    for i in range(4):
        for j in range(4):
            if j != i:
                yield points[j]


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


arr1 = [
    [0, 0, 0, 8, 3, 3],  # body
]
arr11 = [
    [7, 0, 3, 11, 3, 6],  # head
]
arr2 = [
    [7, 0, 6, 8, 1, 7],  # left ear
    [7, 3, 6, 8, 2, 7],  # right ear
]
arr3 = [
    [-3, 1, 2, 0, 2, 3],  # tail
    [-3, 1, 3, -2, 2, 5],  # tail
    [-4, 1, 4, -3, 2, 5],  # tail
    [7, 0, -1, 8, 1, 0],
    [7, 2, -1, 8, 3, 0],
    [0, 2, -1, 1, 3, 0],
    [0, 0, -1, 1, 1, 0]
]

body_color = (1, 0.84, 0)
head_color = (1, 0.9, 0.05)
other_color = (0.32, 0.26, 0.1)
colors = []
points = []

for elem in arr1:
    colors.append([])
    points.append([])
    for e1 in cube_from_2p(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5]):
        points[-1].append((e1[0], e1[1], e1[2]))
        # print("{}.0f,{}.0f,{}.0f,".format(e1[0], e1[1], e1[2]), end=" ")
        colors[-1].append(body_color)
    print()

for elem in arr11:
    colors.append([])
    points.append([])
    for e1 in cube_from_2p(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5]):
        points[-1].append((e1[0], e1[1], e1[2]))
        # print("{}.0f,{}.0f,{}.0f,".format(e1[0], e1[1], e1[2]), end=" ")
        colors[-1].append(head_color)
    print()

for elem in arr2:
    colors.append([])
    points.append([])
    for e1 in half_of_cube_to_triangle(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5]):
        points[-1].append((e1[0], e1[1], e1[2]))
        # print("{}.0f,{}.0f,{}.0f,".format(e1[0], e1[1], e1[2]), end=" ")
        colors[-1].append(other_color)
    print()

for elem in arr3:
    colors.append([])
    points.append([])
    for e1 in cube_from_2p(elem[0], elem[1], elem[2], elem[3], elem[4], elem[5]):
        points[-1].append((e1[0], e1[1], e1[2]))
        # print("{}.0f,{}.0f,{}.0f,".format(e1[0], e1[1], e1[2]), end=" ")
        colors[-1].append(other_color)
    print()

for elem in points:
    for i in range(0, len(elem), 3):
        tr1 = elem[i]
        tr2 = elem[i + 1]
        tr3 = elem[i + 2]
        print("Triangle({", "{}.0f,{}.0f,{}.0f, {}.0f,{}.0f,{}.0f, {}.0f,{}.0f,{}.0f,".format(tr1[0], tr1[1], tr1[2],
              tr2[0], tr2[1], tr2[2], tr3[0], tr3[1], tr3[2]), "}),", sep="")
    print()

        # print("-----")
        # for elem in colors:
        # 	for c in elem:
        # 		print(c[0], c[1], c[2], end=", ", sep=", ")
        # 	print()

