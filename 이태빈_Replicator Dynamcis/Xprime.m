function dxdt = Xprime(t, x)  % 연립방정식 푸는 함수
global Cwm Cce Cwl N1 N2 N3  %전역변수로 설정 (상수 값)

dxdt = zeros(3,1); % Xwm2Prime, Xwm3Prime, Xce3Prime 출력 버퍼 설정
dxdt(1) = ( ( x(1)- x(1)*x(1) )  * ( Pwm2() - Pce2() ) );  % Xwm2Prime
dxdt(2) = x(2) * ( Pwm3() - x(2)*Pwm3() - x(3)* Pce3()  - (1-x(2)-x(3))*Pwl3() ); % Xwm3Prime
dxdt(3) = x(3) *  ( Pce3() - x(2)*Pwm3() - x(3)* Pce3()   - (1-x(2)-x(3))*Pwl3() );  % Xce3Prime
        
    function Pwm2 = Pwm2()   % (2)area에서 "WMAN"을 선택했을 때의 Payoff 
     nwm = N1 + (N2 * x(1) ) + (N3 * x(2) ); 
     Pwm2 = Cwm/nwm - 0.01 * ( (N2 * x(1)) + (N3 * x(2) ) );
    end

    function Pwm3 = Pwm3()   % (3)area에서 "WMAN"을 선택했을 때의 Payoff 
    nwm = N1 + (N2 * x(1) ) + (N3 * x(2) ); 
    Pwm3 = Cwm/nwm - 0.01 * (N3 * x(2)); 
    end

    function Pce2 = Pce2()   % (2)area에서 "Cellular"를 선택했을 때의 Payoff 
    nce = (N2 * ( 1-x(1))  + (N3 * x(3)) );
    Pce2 = Cce/nce - 0.01 * nce;       
    end

    function Pce3 = Pce3()  % (3)area에서 "Cellular"를 선택했을 때의 Payoff
    nce = (N2 * ( 1-x(1))  + (N3 * x(3)) );
    Pce3 = Cce/nce - 0.01 * ( N3 * x(3) );       
    end

    function Pwl3 = Pwl3()  % (2)area에서 "WLAN"을 선택했을 때의 Payoff
        nwl = N3 * (1- x(2)- x(3) );
        Pwl3 = Cwl/nwl - 0.01 * nwl;
    end
end

