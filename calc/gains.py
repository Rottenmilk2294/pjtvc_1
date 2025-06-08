import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

m = 1.0 #mass kg
I = 0.1 #moment of inertia kg*m^2
T = 20.0 #thrust newtons
L = 0.1 #distance from cg to gimbal
cp_cg = 0.05 #cp-cg needs to be POSTIVE value for stability

Kp = 5.0
Kd = 1.0

def rocketDynamics(state, t):
    theta, omega = state #tilt and angular velocity
    
    delta = Kp * (0 - theta) + Kd * (0 - omega) #proprotional control for gimbal delta
    #deltaValues.append(delta)

    torqueThrust = T * L * delta
    torqueGravity = m * 9.81 * cp_cg * theta
    torqueNet = torqueThrust - torqueGravity

    alpha = torqueNet / I

    return [omega, alpha]

theta0 = 0.1 #0.1 radian
omega0 = 0.0 #initial angular velocity
t = np.linspace(0, 2, 1000)
#t = np.arange(0, len(rocketDynamics), 1)

#deltaValues = []

solution = odeint(rocketDynamics, [theta0, omega0], t)
theta, omega = solution.T

plt.figure(figsize=(10, 6))
plt.plot(t, theta, label='Tilt angle (theta)')
plt.plot(t, omega, label='Angular velocity (w)')
#plt.plot(t, deltaValues, '--', label='Gimbal angle (δ)')
plt.xlabel('seconds')
plt.ylabel('Angle rad / Velocity rad/s')
plt.title('Rocket stability')
plt.legend()
plt.grid()
plt.show()
