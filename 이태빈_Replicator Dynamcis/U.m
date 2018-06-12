function [Utility] = U(C, nwm)
    if nwm == 0  % 분모가 0일 수는 없으므로, 0일 때 예외처리
        Utility = 0;
    else
        Utility = C./nwm;
    end
end
