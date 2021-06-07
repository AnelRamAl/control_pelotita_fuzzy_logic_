clear all
close all
clc

%%%%%%%%%%%%%%%%%%
%%%%Ruido real%%%%
%%%%%%%%%%%%%%%%%%

% t_rea=xlsread('Prueba3.xlsx',1,'B1:B6000');
% v_rea=xlsread('Prueba3.xlsx',1,'A1:A6000');
% 
% medr= xlsread('Prueba3.xlsx',1,'E8');
% varr = xlsread('Prueba3.xlsx',1,'E11');

t_rea=xlsread('Prueba10.xlsx',1,'B1:B6000');
v_rea=xlsread('Prueba10.xlsx',1,'A1:A6000');

medg= xlsread('Prueba10.xlsx',1,'E9');
varg = xlsread('Prueba10.xlsx',1,'E12');

figure(1)
hold on
plot(t_rea,v_rea)
xlim([t_rea(1) t_rea(end)])
ylim([10 30])

% t_gen=xlsread('Prueba3.xlsx',1,'B1:B6000');
% v_gen=xlsread('Prueba3.xlsx',1,'A1:A6000');
% 
% medg= xlsread('Prueba3.xlsx',1,'E8');
% varg = xlsread('Prueba3.xlsx',1,'E11');

t_gen=xlsread('Prueba10.xlsx',1,'B1:B6000');
v_gen=xlsread('Prueba10.xlsx',1,'A1:A6000');

medg= xlsread('Prueba10.xlsx',1,'E9');
varg = xlsread('Prueba10.xlsx',1,'E12');

figure(2)
[f3,x3]=hist(v_gen,100);
bar(x3,f3/sum(f3),'r')
hold on
g3_aux=(1/sqrt(2*pi*varg))*exp((-1/2)*((x3-medg).^2)/varg);
g3=g3_aux/sum(g3_aux);
plot(x3,g3,'k')
