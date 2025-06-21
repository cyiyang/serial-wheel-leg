    i=0.1;
    j=1;
    k=0.1:0.005:0.295;
    lw=zeros(1,40);
    while i<=0.3
        lw(j) = double(get_lw(i));
        i=i+0.005;
        j=j+1;
    end
    [ak,s]=polyfit(k,lw,2);
    lwf=polyval(ak,k);
    figure;
    plot(k,lw,'o',k,lwf);xlabel('L0');ylabel('lw');legend('real','fitting');
    title('lw quadratic fit')