%T = readtable('Q2.csv', 'HeaderLines',1);  % skips the first three rows of data
Table = csvread('Q2.csv',1);  % skips the first three rows of data

x = Table(:,5);
y = Table(:,6);

plot(x,y,'b-o');

title('Packet Arrival rate vs. Mean Delay');
grid on;
set(gca,'Color','w')
set(gcf,'color','w');
xlabel('Arrival rate (Packets/sec)') 
ylabel('Mean Delay (msec)') 

