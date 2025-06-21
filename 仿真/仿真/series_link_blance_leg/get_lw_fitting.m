function ak = get_lw_fitting()
    i=0.1;
    j=1;
    lw=zeros(1,40);
    k=0.1:0.005:0.295;
    while i<=0.3
        lw(j) = double(get_lw(i));
        i=i+0.005;
        j=j+1;
    end
    ak=polyfit(k,lw,2);
end
