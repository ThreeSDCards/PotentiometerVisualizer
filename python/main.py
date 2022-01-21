from serial import Serial
from collections import deque
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Initializeer Serial lijn
com_line = 'COM6'
baud_rate = 19200
s = Serial(com_line, baud_rate)
s.close()

#Initializeer grafiek variabelen
x = [x for x in range(-150, 20)]
y = deque([0 for y in range(-150, 20)])
fig = plt.figure()
ax = plt.axes(xlim=(-150, 20), ylim=(0, 1023))
line, = ax.plot([], [], lw=2)


# initialization functie van grafiek
def init():
    line.set_data([x for x in range(-150,20)], y)
    return line,

# Neemt meest recente leesbare waarde
def this_value():
    while True:
        l = s.readline()
        try:
            if len(l.strip()) == 3:
                return int(l.strip().decode('unicode_escape'), 16)
        except ValueError:
            pass



def animate(i):
    s.open()
    new_val = this_value()
    s.close()

    y.rotate(-1)
    for i in range(150, 170):
        y[i] = new_val

    line.set_data(x, y)
    return line,


anim = FuncAnimation(fig, animate, init_func=init,
           frames=200, interval=20, blit=False)
plt.show()
