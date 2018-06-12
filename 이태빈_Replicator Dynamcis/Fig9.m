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

 t= [0 1];  % 시간축 0부터 1으로 설정
 x0 = [0.7  0.7  0];  % 초기값 Xwm2=0.7 , Xwm3=0.7, Xce3 =0 으로 설정

[T, X] = ode45(@Xprime, t , x0) ; % 연립미분 방정식에서, 해를 구해주는 함수

sampleNumber = 61;  % Sample 개수를 61개만 받아서 보기 위함 약 32000개를 다 받아오면 그림이 직관적이지 않기 떄문
 time = zeros(1,100);  % 시간 설정
 n = 1;  % index
 
 for i = 0: 0.5 : sampleNumber/2
     time(n) = i;
     n = n+1;
 end
 
 size(X)
x=X(1:sampleNumber,1);  % Samplenumber수만큼 Xwm2의 값 사용
y=X(1:sampleNumber,2);  % Samplenumber수만큼 Xwm3의 값 사용
z=X(1:sampleNumber,3);  % % Samplenumber수만큼 Xce3의 값 사용

    xwm2 = x;   % Xwm(2)를 x축으로 설정
    xce2 = 1-x;  % Xce(2)는 x로 표현 가능
    xwm3 = y;   % Xwm(3)를 y축으로 설정
    xce3 = z;    % 초기 설정        
    xwl3 = 1-y-z;  % Xwl(3)는 y로 표현 가능
    
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
    
     plot(time(1,1:sampleNumber), Pwm2, 'b', 'LineWidth', 2), % Payoff selecting WMAN in area2
     hold on,
     plot(time(1,1:sampleNumber), Pce2, 'r', 'LineWidth', 2), % Payoff selecting Cellular in area2
     
    xlabel('Time'); % x축 naming   -> 반복횟수
    ylabel('Payoff값'); % y축 naming  
    legend('Payoff selecting WMAN in area2','Payoff selecting Cellular in area2')  % 그래프에 naming