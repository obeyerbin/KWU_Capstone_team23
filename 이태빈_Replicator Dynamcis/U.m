function [Utility] = U(C, nwm)
    if nwm == 0  % �и� 0�� ���� �����Ƿ�, 0�� �� ����ó��
        Utility = 0;
    else
        Utility = C./nwm;
    end
end
