import matplotlib.pyplot as plt
import serial

def setup_backend(backend='TkAgg'):
	import sys
	del sys.modules['matplotlib.backends']
	del sys.modules['matplotlib.pyplot']
	import matplotlib as mpl
	mpl.use(backend)
	import matplotlib.pyplot as plt
	return plt

def animate():
	rects = plt.bar(0, 3000, align='center', width=1)
	while(True):
		for rect in rects:
			rect.set_height(int(ser.readline()));
			fig.canvas.draw()

ser = serial.Serial(7)
print ser.name
plt = setup_backend()
fig = plt.figure()
win = fig.canvas.manager.window
win.after(10, animate)
plt.show()
