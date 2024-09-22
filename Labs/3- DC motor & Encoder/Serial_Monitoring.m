clc;clear;
close all;
T = 2; % total sampling time
fs = 100; % sampling frequency
Ts = 1/fs; % sampling period
n = T * fs; % #samples

% connecting to Arduino
Serial_monitor = serialport('COM13',115200);
configureTerminator(Serial_monitor,'CR/LF');
stepRes = zeros(n,1);

% saving the data
for i = 1:n
 stepRes(i,:) = double(strsplit(readline(Serial_monitor)));
end

% plotting step response
time = (0: 1/fs : (n-1)/fs)';
figure
plot(time , stepRes,DisplayName='Step Response',LineWidth=1);
title('Experimental Step Response');
yline(1000,DisplayName='Reference',LineWidth=1,Color='red')
legend
clear Serial_monitor