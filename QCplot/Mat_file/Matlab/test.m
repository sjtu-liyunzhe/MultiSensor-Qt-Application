function [a,b]=test(value)
a=value+1;
b=value+3;
% value=a;
opts = struct('WindowStyle','modal',... 
              'Interpreter','tex');
warndlg('\fontsize{15}\color{red} 成功',...
             '提示', opts);
end