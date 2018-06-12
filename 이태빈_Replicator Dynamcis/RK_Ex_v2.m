 clear;clc;close all;

global Cwm Cce Cwl N1 N2 N3 
Cwm = 10e6; % WMAN total Capacity ( 10Mbps)    
Cce = 2e6;  % Celluar total Capacity (2Mbps)
Cwl = 7e6;  % WLAN total Capacity (7Mbps)
N1 = 10;    % area1의 user 수
N2 = 10;    % area2의 user 수
N3 = 30;    % area3의 user 수

 xwm1 = 1;      % 1
 SIGMA = 1;     % 시그마 값 =1
 Price_wm = 0.01;  % pricing 0.01로 통일
 Price_ce = 0.01;  % pricing 0.01로 통일
 Price_wl = 0.01;  % pricing 0.01로 통일

 t= [0 0.1];  % 시간축 0부터 30으로 설정
 x0 = [0.7  0.7  0.0];  % 초기값 Xwm2=0.7 , Xwm3=0.7, Xce3 =0 으로 설정

[T, X] = ode45(@Xprime, t , x0) ; % 연립미분 방정식에서, 룽게쿠타4,5차를 이용하여 해를 구해주는 함수

[x,y]= meshgrid(0.1 : 0.05: 1, 0.1 : 0.05 : 1);  % 0.1~0.95까지 0.05 간격으로 설정
    xwm2 = x;   % Xwm(2)를 x축으로 설정
    xce2 = 1-x;  % Xce(2)는 x로 표현 가능
    xwm3 = y;   % Xwm(3)를 y축으로 설정
    xce3 = 0;    % 초기 설정        
    xwl3 = 1-y;  % Xwl(3)는 y로 표현 가능
    
    % 해당 rat을 선택한 user들의 수
    nwm = N1 + (N2 * xwm2) + (N3 * xwm3); 
    nce = (N2 * xce2) + (N3 * xce3);
    nwl = N3 * xwl3;              
    
    % Payoff 값 구하는 부분 (Capacity의 Subarea 포함한 개념) 
    % U 함수  = Capacity(i) / n(i)
    Pwm1 = U(Cwm, nwm) - Price_wm * nwm;  % (1)번 area에서 WMAN 선택했을 때 payoff
    Pwm2 = U(Cwm, nwm) - Price_wm * ((N2 * xwm2) + (N3 * xwm3)); % (2)번 area에서 WMAN 선택했을 때 payoff
    Pwm3 = U(Cwm, nwm) - Price_wm * (N3 * xwm3);  % (3)번 area에서 WMAN 선택했을 때 payoff
    Pce2 = U(Cce, nce) - Price_ce * nce;         % (2)번 area에서 Celluar 선택했을 때 payoff
    Pce3 = U(Cce, nce) - Price_ce * (N3 * xce3); % (3)번 area에서 Celluar 선택했을 때 payoff
    Pwl3 = U(Cwl, nwl) - Price_wl * nwl;     % (3)번 area에서 WMAN 선택했을 때 payoff

    % payoff값 평균 (해당 area에서 Xi*Pi의 합)
    Pavrg1 = Pwm1;  
    Pavrg2 = (xwm2 .* Pwm2) + (xce2 .* Pce2);  
    Pavrg3 = (xwm3 .* Pwm3) + (xce3 .* Pce3) + (xwl3 .* Pwl3);
    
    % 논문의  (4) 식에 대입
    xwm1PRIME = SIGMA .* xwm1 .* (Pwm1 - Pavrg1);  % 사실 상 0임. because  Pwm1 = Pavrg1
    xwm2PRIME = SIGMA .* xwm2 .* (Pwm2 - Pavrg2);
    xwm3PRIME = SIGMA .* xwm3 .* (Pwm3 - Pavrg3);            
    xce2PRIME = SIGMA .* xce2 .* (Pce2 - Pavrg2);
    xce3PRIME = SIGMA .* xce3 .* (Pce3 - Pavrg3); 
    xwl3PRIME = SIGMA .* xwl3 .* (Pwl3 - Pavrg3);
    
    % 화살표 크기를 모두 1로 통일하기 위해 단위 벡터로 바꾸는 부분
    ssize = 0.04 ./ (xwm2PRIME.^2 + xwm3PRIME.^2).^(1/2);  
    xwm2PRIME = ssize .* xwm2PRIME;
    xwm3PRIME = ssize .* xwm3PRIME;
    
    size(X)
     x2=X(:,1);  % Xwm2의 값들을 x2에 받기
     x3=X(:,2);  % Xwm3의 값들을 x3에 받기
    
    figure                     
    quiver(x, y, xwm2PRIME, xwm3PRIME, 0); % quiver함수를 이용하여 Xwm2, Xwm3 벡터(x,y) 표시
    hold on, axis tight,  % axis tight은 plot할때 Figure에 그래프가 꽉 차게 하기 위함
    plot(x2, x3, 'r', 'LineWidth', 2), axis([0.1 0.95 0.1 0.95]) % trajectory를 빨간선 으로 그리기
    hold off, 
    xlabel('Xwm^(^2^)'); % x축 naming   -> 0.475 로 수렴
    ylabel('Xwm^(^3^)'); % y축 naming   -> 0.385 로 수렴
    legend('Vector values of Xwm(2),Xwm(3)','Trajectory of replicator dynamics')  % 그래프에 naming