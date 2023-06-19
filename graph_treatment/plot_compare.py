#this file will be used to plot the different elements 
import numpy as np 
import matplotlib.pyplot as plt


plt.figure(1)

dt_align = np.loadtxt("res_100_paris_align")
dt_attra = np.loadtxt("res_100_paris_attra")
dt_attco = np.loadtxt("res_100_paris_attra_coeff")
dt_noisy_align = np.loadtxt("res_100_paris_noisy_align")
dt_noisy_attra = np.loadtxt("res_100_paris_noisy_attra")

plt.plot(dt_align.T[0], color='r', label='align')
plt.plot(dt_attra.T[0], color='y', label='attra')
plt.plot(dt_attco.T[0], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[0], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[0], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of number of walker groups")
plt.show()


plt.figure(2)

plt.plot(dt_align.T[1], color='r', label='align')
plt.plot(dt_attra.T[1], color='y', label='attra')
plt.plot(dt_attco.T[1], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[1], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[1], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of mean number of walkers per group")
plt.show()

plt.figure(3)

plt.plot(dt_align.T[2], color='r', label='align')
plt.plot(dt_attra.T[2], color='y', label='attra')
plt.plot(dt_attco.T[2], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[2], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[2], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of total number of groups")
plt.show()


plt.figure(4)

plt.plot(dt_align.T[3], color='r', label='align')
plt.plot(dt_attra.T[3], color='y', label='attra')
plt.plot(dt_attco.T[3], color='g', label='attra coeff')
plt.plot(dt_noisy_align.T[3], color='blue', label='noisy align')
plt.plot(dt_noisy_attra.T[3], color='purple', label='noisy attra')

plt.legend()
plt.title("evolution of the mean of different nodes visited by each walker")
plt.show()