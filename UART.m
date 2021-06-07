close all;
clear;
 clc;
RESPALDO = 0;
n=0;
VECTORFINAL=[];
s = serial('COM8','BaudRate',9600);
fopen(s);
cnt=0;
% p=0;
while cnt<6000
%M=zeros(1,cnt+1);   
LECTURA=fread(s,1);
VECTORFINAL=cat(1,VECTORFINAL,LECTURA);

pause(0.01);
%  for a=1:cnt+1
%      M(1,a)= p;
%      p=p+1;
%  end
%  p=0;
%  plot(M,VECTORFINAL)
%  drawnow
cnt = cnt + 1
end

fclose(s);
M=zeros(1,cnt);
p=0;
for a=1:cnt
    M(1,a)= p;
    p=p+1;
end
plot(M,VECTORFINAL)
title('Gráfica de Balanceo de Pelota')
xlabel('No de Muestras')
ylabel('Distancia (cm)')