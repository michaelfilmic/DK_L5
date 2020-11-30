Table = csvread('Q1b_5.csv',1);  % skips the first three rows of data

x = Table(1:15,1);
output_rate = Table(1:15,5);
block_rate = Table(1:15,3);

yyaxis left
plot(x,block_rate,'b-o','LineWidth',3);
ylabel('Blocking rate') 
hold on;

yyaxis right
plot(x,output_rate,'r-o','LineWidth',3);
ylabel('Output rate (Mbps)')

hold off;

title('Link rate vs. Output rate (Mbps) and Blocking rate');
legend('Blocking rate', 'Output rate');
grid on;
set(gca,'Color','w')
set(gcf,'color','w');
xlabel('Link Rate') 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Table = csvread('Q1a_5.csv',1);  % skips the first three rows of data
% 
% x = Table(:,1);
% output_rate = Table(:,5);
% block_rate = Table(:,3);
% 
% yyaxis left
% plot(x,block_rate,'b-o','LineWidth',3);
% ylabel('Blocking rate') 
% hold on;
% 
% yyaxis right
% ylim([0 0.01])
% plot(x,output_rate,'r-o','LineWidth',3);
% ylabel('Output rate (Mbps)')
% 
% hold off;
% 
% title('Max queue size vs. Output rate (Mbps) and Blocking rate');
% legend('Blocking rate', 'Output rate');
% grid on;
% set(gca, 'XScale', 'log')
% set(gca,'Color','w')
% set(gcf,'color','w');
% xlabel('Max queue size') 
