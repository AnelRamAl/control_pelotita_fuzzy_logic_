clc 
clear 
y1 = wgn(10000,1,0);
a=min(y1);
b=max(y1);
x=a:0.1:b;
subplot(1,2,1)
hist(y1,x)
grid on
subplot(1,2,2)
plot(y1)
grid on



