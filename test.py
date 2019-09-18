print("*** the goal of this program is to be annoying")
w = "yeet"
i = 0
while i < 20:
    if i == 10:
        print("*** we are halfway there!")
    elif i < 10:
        print("*** we are almost halfway there!")
    else:
        print("*** we are more than halfway there!")
    print("***" + w)
    w += "yeet"
    i += 1
print("*** we are done")