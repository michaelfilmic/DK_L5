Table = csvread('Q2.csv',1);  % skips the first three rows of data

x = Table(:,1);
output_rate = Table(:,5);
block_rate = Table(:,3);

yyaxis left
plot(x,block_rate,'b-o','LineWidth',3);
ylabel('Blocking rate') 
hold on;

yyaxis right
plot(x,output_rate,'r-o','LineWidth',3);
ylabel('Output rate (Mbps)') 
hold off;

title('n vs. Output rate (Mbps) and Blocking rate');
legend('Blocking rate', 'Output rate');
grid on;
set(gca,'Color','w')
set(gcf,'color','w');
xlabel('n') 

