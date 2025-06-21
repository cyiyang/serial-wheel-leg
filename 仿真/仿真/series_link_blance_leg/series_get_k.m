%计算不同腿长下适合的K矩阵，再进行多项式拟合
tic
j=1;
leg=0.1:0.01:0.4;
ak = get_lw_fitting();
for i=leg
    k=series_get_k1_length(i,ak);
    k11(j) = k(1,1);
    k12(j) = k(1,2);
    k13(j) = k(1,3);
    k14(j) = k(1,4);
    j=j+1;
end
a11=polyfit(leg,k11,2);
a12=polyfit(leg,k12,2);
a13=polyfit(leg,k13,1);
a14=polyfit(leg,k14,2);


x0=leg;              %步长为0.1
y11=polyval(a11,x0);          %返回值y0是对应于x0的函数值
y12=polyval(a12,x0);          %返回值y0是对应于x0的函数值
y13=polyval(a13,x0);          %返回值y0是对应于x0的函数值
y14=polyval(a14,x0);          %返回值y0是对应于x0的函数值
subplot(2,2,1);plot(leg,k11,'o',x0,y11,'r');xlabel('x');ylabel('y');title('k11');legend('real','fitting');
subplot(2,2,2);plot(leg,k12,'o',x0,y12,'r');xlabel('x');ylabel('y');title('k12');legend('real','fitting');
subplot(2,2,3);plot(leg,k13,'o',x0,y13,'r');xlabel('x');ylabel('y');title('k13');legend('real','fitting');
subplot(2,2,4);plot(leg,k14,'o',x0,y14,'r');xlabel('x');ylabel('y');title('k14');legend('real','fitting');



toc
