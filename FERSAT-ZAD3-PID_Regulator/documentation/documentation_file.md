## FERSat - ADCS group

# PID regulator for one-axis angular velocity regulation for CubeSat satellites

#### On project worked: Karla Sever, Ivan Vnučec and Josip Lončar (mentor)

In this document it will be presented the procedures used for the parametrization of PID regulator which is used for the one-axis regulation of the angular velocity.

## Problem

### Testbed

The main challenge is to develop PID regulator to control one-degree of freedom angular velocity. The test bed used for the testing is shown below. It consists of 3D printed air-bearing and plastic sphere which holds the satellite. In plastic sphere there is main control board with Arduino microcontroller development platform, IMU (Inertial measurement unit) sensor and one Momentum wheel controlling angular velocity about one-axis.  

![Picture 1: Testbed](C:\Users\Ivan\Documents\GitHub\FERSAT-ZAD3-PID-Regulator\documentation\dependencies\Screenshot_1.png)

### PID regulator

As for PID regulator we used the parallel type one with the equation: $u(t)=K_{p}e(t)+K_{i}\int_{0}^{t}e(t^{'})dt^{'}+K_{d}\frac{de(t))}{dt}$

## Solution

Firstly, we started with [Ziegler–Nichols method](https://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method) to determine the 