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

 t= [0 1];  % �ð��� 0���� 1���� ����
 x0 = [0.7  0.7  0];  % �ʱⰪ Xwm2=0.7 , Xwm3=0.7, Xce3 =0 ���� ����

[T, X] = ode45(@Xprime, t , x0) ; % �����̺� �����Ŀ���, �ظ� �����ִ� �Լ�

sampleNumber = 61;  % Sample ������ 61���� �޾Ƽ� ���� ���� �� 32000���� �� �޾ƿ��� �׸��� ���������� �ʱ� ����
 time = zeros(1,100);  % �ð� ����
 n = 1;  % index
 
 for i = 0: 0.5 : sampleNumber/2
     time(n) = i;
     n = n+1;
 end
 
 size(X)
x=X(1:sampleNumber,1);  % Samplenumber����ŭ Xwm2�� �� ���
y=X(1:sampleNumber,2);  % Samplenumber����ŭ Xwm3�� �� ���
z=X(1:sampleNumber,3);  % % Samplenumber����ŭ Xce3�� �� ���

    xwm2 = x;   % Xwm(2)�� x������ ����
    xce2 = 1-x;  % Xce(2)�� x�� ǥ�� ����
    xwm3 = y;   % Xwm(3)�� y������ ����
    xce3 = z;    % �ʱ� ����        
    xwl3 = 1-y-z;  % Xwl(3)�� y�� ǥ�� ����
    
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
    
     plot(time(1,1:sampleNumber), Pwm2, 'b', 'LineWidth', 2), % Payoff selecting WMAN in area2
     hold on,
     plot(time(1,1:sampleNumber), Pce2, 'r', 'LineWidth', 2), % Payoff selecting Cellular in area2
     
    xlabel('Time'); % x�� naming   -> �ݺ�Ƚ��
    ylabel('Payoff��'); % y�� naming  
    legend('Payoff selecting WMAN in area2','Payoff selecting Cellular in area2')  % �׷����� naming