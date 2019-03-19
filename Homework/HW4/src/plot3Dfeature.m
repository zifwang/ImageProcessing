% This File is use to plot 3D feature vector
data = importdata('texture_3D.txt');
plot3(data(:,1),data(:,2),data(:,3),'.')
grid on