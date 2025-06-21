function K2 = serial_get_K2_length(leg_length)
    K1=series_get_k1_length(leg_length);
    K2=zeros(2,6);
    K2(2,1)=K1(2,1);
    K2(2,2)=K1(2,2);
    K2(2,5)=K1(2,5);
    K2(2,6)=K1(2,6);
end