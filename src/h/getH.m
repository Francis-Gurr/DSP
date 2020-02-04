%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% SUM FILTER
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N    = 172;     % Order
Fc1  = 0.98;     % First Cutoff Frequency
Fc2  = 1.02;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_SUM  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);
H_SUM = fft(H_SUM, 2048);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% DIFF FILTER
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N    = 172;     % Order
Fc1  = 1.03;     % First Cutoff Frequency
Fc2  = 1.07;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_DIFF  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);
H_DIFF = fft(H_DIFF, 2048);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N    = 172;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW  = fir1(N, Fc/(Fs/2), 'low', win, flag);
H_LOW = fft(H_LOW, 2048);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% RESAMPLING FILTER
% All frequency values are in MHz.
Fs = 30;  % Sampling Frequency
N_RES = 512
N    = N_RES*6 - 1;     % Order
Fc  = 0.024;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_RES  = fir1(N, Fc/(Fs/2), 'low', win, flag);
% Split filter
H_RES = reshape(H_RES,[6,N_RES]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% OSC
for i = 1:200
	OSC(i) = cos(((i-1)/100)*pi);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.c FILE
M = 173;
N = 2048;

fileID = fopen('init.c','w');

% % H_SUM
% fprintf(fileID,'const double H[2][%d][2] = {{\n\t', N);
% for i = 1:N-1
%     fprintf(fileID,'{%.10f, %.10f},', real(H_SUM(i)), imag(H_SUM(i)));
%     if mod(i,15)==0
%         fprintf(fileID,'\n\t');
%     end
% end
% fprintf(fileID,'{%.10f, %.10f}},\n\t\t{', real(H_SUM(N)), imag(H_SUM(N)));
% 
% % H_DIFF
% for i = 1:N-1
%     fprintf(fileID,'{%.10f, %.10f},', real(H_DIFF(i)), imag(H_DIFF(i)));
%     if mod(i,15)==0
%         fprintf(fileID,'\n\t');
%     end
% end
% fprintf(fileID,'{%.10f, %.10f}}};\n\n', real(H_DIFF(i)), imag(H_DIFF(i)));

% H_LOW
fprintf(fileID,'const double H_LOW[%d][2] = {', N);
for i = 1:N-1
    fprintf(fileID,'{%.15f, %.15f},', real(H_LOW(i)), imag(H_LOW(i)));
    if mod(i,8)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'{%.15f, %.15f}};\n\n', real(H_LOW(N)), imag(H_LOW(N)));


% H_RES
fprintf(fileID,'const double H_RES[6][%d] = {', N_RES);
for i = 1:6
    fprintf(fileID,'{');
    for j = 1:N_RES-1
        fprintf(fileID,'%.15f, ', H_RES(i, j));
        if mod(j,8)==0
            fprintf(fileID,'\n\t\t');
        end
    end
    if i==6
        fprintf(fileID,'%.15f}};\n\n', H_RES(i, N_RES));
    else
        fprintf(fileID,'%.15f},\n\t\t', H_RES(i, N_RES));
    end
end
% fprintf(fileID,'void get_H_RES(double *p_H0, double *p_H1, double *p_H2) {\n');
% % H0
% fprintf(fileID,'\t*p_H0 = %.15f;\n', H0(1));
% for i = 2:N
%     fprintf(fileID, '\t*(p_H0 + %d) = %.15f;\n', i-1, H0(i));
% end
% fprintf(fileID,'\n');
% % H1
% fprintf(fileID,'\t*p_H1 = %.15f;\n', H1(1));
% for i = 2:N
%     fprintf(fileID, '\t*(p_H0 + %d) = %.15f;\n', i-1, H1(i));
% end
% fprintf(fileID,'\n');
% % H2
% fprintf(fileID,'\t*p_H0 = %.15f;\n', H2(1));
% for i = 2:N
%     fprintf(fileID, '\t*(p_H0 + %d) = %.15f;\n', i-1, H2(i));
% end
% fprintf(fileID,'}');

% % OSC
fprintf(fileID,'const double OSC[200] = {');
for i = 1:199
    fprintf(fileID,'%.15f,', OSC(i));
    if mod(i,20)==0
        fprintf(fileID,'\n\t');
    end
end
fprintf(fileID,'%.15f};', OSC(200));

fclose(fileID);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.h FILE
fileID = fopen('init.h','w');
fprintf(fileID,'#ifndef _CONSTS\n');
fprintf(fileID,'#define _CONSTS\n\n');
fprintf(fileID,'#define FILTER_LEN %d\n', N);
fprintf(fileID,'#define M_RES %d\n', N_RES);
fprintf(fileID, '\nconst double H_LOW[%d][2];\n', N);
fprintf(fileID, '\nconst double H_RES[6][%d];\n', N_RES);
fprintf(fileID, '\nconst double OSC[200];\n');
fprintf(fileID,'\n#endif\n');

fclose(fileID);