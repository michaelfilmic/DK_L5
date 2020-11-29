clear all;
Table = csvread('Q3_B_t.csv',1);  % it affects

chan_size = 8;
loop_size = 4;

for i = 1 : loop_size
    arrival_rate(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 5);
    B_t(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 7);
    B_d(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 8);
    block_rate(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 9);
    output_rate(:,i) = Table(chan_size*(i-1)+1:chan_size*i, 10);
end

figure(1)
%option to D vs tpt

title('output rate vs. arrival');
hold on
for i = 1 : loop_size
   output_rate_vs_arrival(i) = plot(arrival_rate(:,i), output_rate(:,i), 'LineWidth',1);
end
xlabel('arrival rate');
ylabel('output rate');

title('block rate vs. arrival');
figure(2)
hold on
for i = 1 : loop_size
   block_rate_vs_arrival(i) = plot(arrival_rate(:,i), block_rate(:,i), 'LineWidth',1);
end

xlabel('arrival rate');
ylabel('block rate');

%legend('Backduration = 5', 'Backduration = 10');
%legend('X/X_r = 5/5', 'X/X_r = 10/5', 'X/X_r = 15/5', 'X/X_r = 20/5', 'X/X_r = 25/5');
%legend('X/X_r = 5', 'X/X_r = 5/2', 'X/X_r = 5/3', 'X/X_r = 5/4', 'X/X_r = 5/5');
%legend('# of station = 5', '# of station = 10', '# of station = 15', '# of station = 20', '# of station = 25');
hold off

grid on
%set(gca, 'XScale', 'log');
%ylim([0 15])



