 clear;clc;close all;

global Cwm Cce Cwl N1 N2 N3 
Cwm = 10e6; % WMAN total Capacity ( 10Mbps)    
Cce = 2e6;  % Celluar total Capacity (2Mbps)
Cwl = 7e6;  % WLAN total Capacity (7Mbps)
N1 = 10;    % area1�� user ��
N2 = 10;    % area2�� user ��
N3 = 30;    % area3�� user ��

 xwm1 = 1;      % 1
 SIGMA = 1;     % �ñ׸� �� =1
 Price_wm = 0.01;  % pricing 0.01�� ����
 Price_ce = 0.01;  % pricing 0.01�� ����
 Price_wl = 0.01;  % pricing 0.01�� ����

 t= [0 0.1];  % �ð��� 0���� 30���� ����
 x0 = [0.7  0.7  0.0];  % �ʱⰪ Xwm2=0.7 , Xwm3=0.7, Xce3 =0 ���� ����

[T, X] = ode45(@Xprime, t , x0) ; % �����̺� �����Ŀ���, �����Ÿ4,5���� �̿��Ͽ� �ظ� �����ִ� �Լ�

[x,y]= meshgrid(0.1 : 0.05: 1, 0.1 : 0.05 : 1);  % 0.1~0.95���� 0.05 �������� ����
    xwm2 = x;   % Xwm(2)�� x������ ����
    xce2 = 1-x;  % Xce(2)�� x�� ǥ�� ����
    xwm3 = y;   % Xwm(3)�� y������ ����
    xce3 = 0;    % �ʱ� ����        
    xwl3 = 1-y;  % Xwl(3)�� y�� ǥ�� ����
    
    % �ش� rat�� ������ user���� ��
    nwm = N1 + (N2 * xwm2) + (N3 * xwm3); 
    nce = (N2 * xce2) + (N3 * xce3);
    nwl = N3 * xwl3;              
    
    % Payoff �� ���ϴ� �κ� (Capacity�� Subarea ������ ����) 
    % U �Լ�  = Capacity(i) / n(i)
    Pwm1 = U(Cwm, nwm) - Price_wm * nwm;  % (1)�� area���� WMAN �������� �� payoff
    Pwm2 = U(Cwm, nwm) - Price_wm * ((N2 * xwm2) + (N3 * xwm3)); % (2)�� area���� WMAN �������� �� payoff
    Pwm3 = U(Cwm, nwm) - Price_wm * (N3 * xwm3);  % (3)�� area���� WMAN �������� �� payoff
    Pce2 = U(Cce, nce) - Price_ce * nce;         % (2)�� area���� Celluar �������� �� payoff
    Pce3 = U(Cce, nce) - Price_ce * (N3 * xce3); % (3)�� area���� Celluar �������� �� payoff
    Pwl3 = U(Cwl, nwl) - Price_wl * nwl;     % (3)�� area���� WMAN �������� �� payoff

    % payoff�� ��� (�ش� area���� Xi*Pi�� ��)
    Pavrg1 = Pwm1;  
    Pavrg2 = (xwm2 .* Pwm2) + (xce2 .* Pce2);  
    Pavrg3 = (xwm3 .* Pwm3) + (xce3 .* Pce3) + (xwl3 .* Pwl3);
    
    % ����  (4) �Ŀ� ����
    xwm1PRIME = SIGMA .* xwm1 .* (Pwm1 - Pavrg1);  % ��� �� 0��. because  Pwm1 = Pavrg1
    xwm2PRIME = SIGMA .* xwm2 .* (Pwm2 - Pavrg2);
    xwm3PRIME = SIGMA .* xwm3 .* (Pwm3 - Pavrg3);            
    xce2PRIME = SIGMA .* xce2 .* (Pce2 - Pavrg2);
    xce3PRIME = SIGMA .* xce3 .* (Pce3 - Pavrg3); 
    xwl3PRIME = SIGMA .* xwl3 .* (Pwl3 - Pavrg3);
    
    % ȭ��ǥ ũ�⸦ ��� 1�� �����ϱ� ���� ���� ���ͷ� �ٲٴ� �κ�
    ssize = 0.04 ./ (xwm2PRIME.^2 + xwm3PRIME.^2).^(1/2);  
    xwm2PRIME = ssize .* xwm2PRIME;
    xwm3PRIME = ssize .* xwm3PRIME;
    
    size(X)
     x2=X(:,1);  % Xwm2�� ������ x2�� �ޱ�
     x3=X(:,2);  % Xwm3�� ������ x3�� �ޱ�
    
    figure                     
    quiver(x, y, xwm2PRIME, xwm3PRIME, 0); % quiver�Լ��� �̿��Ͽ� Xwm2, Xwm3 ����(x,y) ǥ��
    hold on, axis tight,  % axis tight�� plot�Ҷ� Figure�� �׷����� �� ���� �ϱ� ����
    plot(x2, x3, 'r', 'LineWidth', 2), axis([0.1 0.95 0.1 0.95]) % trajectory�� ������ ���� �׸���
    hold off, 
    xlabel('Xwm^(^2^)'); % x�� naming   -> 0.475 �� ����
    ylabel('Xwm^(^3^)'); % y�� naming   -> 0.385 �� ����
    legend('Vector values of Xwm(2),Xwm(3)','Trajectory of replicator dynamics')  % �׷����� naming