clear all;
close;
clc;

B = 0;
s=serial('COM8','BaudRate',3000000);
% s=bluetooth('HC-05',1);
fopen(s);

for i=0: 1: 9
    fwrite(s,'1');
    A=fread(s512);
    C = cat(1,B,A);
    B = C;
    i;
end
D = length(B)
t = 0:1:D - 1;
fclose(s);
plot(t,B)
%stem(B)
grid
   
