syms l0 l1 l2 l23 l3 l4 l5 theta1 theta2 theta3 theta4 yc 

l5 = sqrt(l2^2 + l4^2 -2*l2*l4*cos(theta1 + pi/4));
theta2 = asin(l4*sin(theta1 + pi/4)/l5);
theta3 = acos((l23^2+l5^2-l3^2)/(2*l23*l5));
theta4 =theta1 + theta2 +theta3;
yc = l2*sin(theta1) + l1*sin(theta4);
l0 = yc;
J=simplify(jacobian(l0,theta1))


