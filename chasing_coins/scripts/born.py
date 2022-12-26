# -pi<=t<=pi 或 0<=t<=2*pi
# 心形线方程
# x=a*(2*cos(t)-cos(2*t))

# y=a*(2*sin(t)-sin(2*t))
radius = 15
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
t = np.linspace(0,2*np.pi,41)
x = np.array([None]*t.size)
y = np.array([None]*t.size)
for i,theta in enumerate(t):
    y[i] = t[i]
    if i >20:
      radius = 20
    x[i] = radius*np.sin(y[i])
d = pd.DataFrame(np.transpose([x,y*6]),columns={'x_point','y_point'})
d.to_csv('../points/points.csv',index=False)
plt.plot(x,y*6)
# plt.show()
data = str()
for xi,yi,number in zip(x,y*6,np.arange(0,41,1)):
  original = '<model name="coin_{0}">\n\
    <pose>{1} {2} 0 0 0 0</pose>\n\
        <include>\n\
          <static>false</static>\n\
          <uri>model://crazy_coins</uri>\n\
        </include>\n\
    </model>\n'.format(number,xi,yi)
  data += original
coins_data = open('coins_note.sdf','w')
coins_data.write(data)