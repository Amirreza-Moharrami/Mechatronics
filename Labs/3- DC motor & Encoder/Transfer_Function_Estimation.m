clc,clear;
load('stepResponse_Final.mat');
fs = 100; % sampling frequency
Ts = 1/fs; % sampling period
n = 2*fs; % # samples
t = (0: 1/fs : (n-1)/fs)';
u = 200*ones(size(stepRes,1),1); % input vector
np = 1; % # T.F. poles

data = iddata(stepRes,u,Ts) 
sys = tfest(data,np);
figure
plot(t,stepRes); 
title('Experimental Step Response');
Motor = c2d(sys,1/100,'zoh');
sisotool(Motor)