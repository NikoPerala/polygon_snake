from math import sin, cos, pi

def get_polar_circle(points, radius, offset = 0, closed = 0):
    ret = []
    step = (pi * 2) / points

    for i in range(points):
        x = 0.5 + radius * cos(step * i + offset)
        y = 0.5 + radius * sin(step * i + offset)
        ret.append((x, y))
    
    if closed:
        ret.append(ret[0])

    return ret

def write_points(pts, fp):
    for i in range(len(pts) - 1):
        fp.write("w%.2f,%.2f:%.2f,%.2f\n" % (pts[i][0], pts[i][1], pts[i + 1][0], pts[i + 1][1]))

pa1 = 20
pr1 = 0.3
pts1 = get_polar_circle(pa1, pr1)

pa2 = 10
pr2 = 0.15
pts2 = get_polar_circle(pa2, pr2, pi)

data = [pts1, pts2]
closed = [0, 0, 1]
with open("level.lvl", "w+") as fp:
    fp.write("/\n")
    fp.write("W%s\n" % (pa1 + pa2))
    for i in range(len(data)):
        write_points(data[i], fp)
