
% x = [0.07 0.12];
% y = [-0.224 -0.190];
% 
% p = polyfit(x,y,1);
% k = p(1)
% b = p(2)

% 数据
leg = 0.07:0.01:0.12;
medium_num = [-0.224 -0.219 -0.210 -0.203 -0.196 -0.190];

% 线性拟合 (一次方程 y = a*x + b)
p = polyfit(leg, medium_num, 1);
a = p(1); % 斜率
b = p(2); % 截距

% 生成拟合曲线上的点
leg_fit = linspace(min(leg), max(leg), 100);
medium_num_fit = polyval(p, leg_fit);

% 绘制结果
figure;
hold on;
plot(leg, medium_num, 'bo', 'MarkerFaceColor', 'b', 'DisplayName', '数据点'); % 原始数据点
plot(leg_fit, medium_num_fit, 'r-', 'LineWidth', 2, 'DisplayName', sprintf('拟合直线: y = %.3f*x + %.3f', a, b)); % 拟合直线
xlabel('leg');
ylabel('medium\_num');
title('机械中值拟合');
legend('Location', 'best');
grid on;
hold off;

% 显示拟合方程
disp(['拟合方程为: y = ', num2str(a), ' * x + ', num2str(b)]);
