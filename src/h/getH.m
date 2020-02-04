Beta = 4.54;     % Window Parameter

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% BAND PASS FILTER - SUM
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N    = 172;     % Order
Fc1  = 0.98;     % First Cutoff Frequency
Fc2  = 1.02;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_SUM  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);
H_SUM = fft(H_SUM, 2048);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% BAND PASS FILTER - DIFF
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N    = 172;     % Order
Fc1  = 1.03;     % First Cutoff Frequency
Fc2  = 1.07;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_DIFF  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);
H_DIFF = fft(H_DIFF, 2048);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - COSTAS 1
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_DEMOD = 10;     % Order
Fc  = 1.9;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_DEMOD+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_DEMOD_1  = fir1(N_DEMOD, Fc/(Fs/2), 'low', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - COSTAS 2
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_DEMOD    = 10;     % Order
Fc  = 2;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_DEMOD+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_DEMOD  = fir1(N_DEMOD, Fc/(Fs/2), 'low', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - SMALL
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_S    = 172;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_S+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_S  = fir1(N_S, Fc/(Fs/2), 'low', win, flag);
H_LOW_S = fft(H_LOW_S, 2048);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - MEDIUM
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_M  = 345;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_M+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_M  = fir1(N_M, Fc/(Fs/2), 'low', win, flag);
H_LOW_M = fft(H_LOW_M, 4096);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - LARGE
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_L    = 691;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_L+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_L  = fir1(N_L, Fc/(Fs/2), 'low', win, flag);
H_LOW_L = fft(H_LOW_L, 8192);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% RESAMPLING FILTER - SMALL
% All frequency values are in MHz.
Fs = 30;  % Sampling Frequency
N_RES_S = 128
N    = N_RES_S*6 - 1;     % Order
Fc  = 0.024;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_RES  = fir1(N, Fc/(Fs/2), 'low', win, flag);
% Split filter
H_RES_S = reshape(H_RES,[6,N_RES_S]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% RESAMPLING FILTER - MEDIUM
% All frequency values are in MHz.
Fs = 30;  % Sampling Frequency
N_RES_M = 512
N    = N_RES_M*6 - 1;     % Order
Fc  = 0.024;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_RES  = fir1(N, Fc/(Fs/2), 'low', win, flag);
% Split filter
H_RES_M = reshape(H_RES,[6,N_RES_M]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% RESAMPLING FILTER - LARGE 
% All frequency values are in MHz.
Fs = 30;  % Sampling Frequency
N_RES_L = 1024 
N    = N_RES_L*6 - 1;     % Order
Fc  = 0.024;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_RES  = fir1(N, Fc/(Fs/2), 'low', win, flag);
% Split filter
H_RES_L = reshape(H_RES,[6,N_RES_L]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% OSC
for i = 1:200
	OSC(i) = cos(((i-1)/100)*pi);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.c FILE
% LOW PASS - SMALL
M = 173;
N = 2048;
H_LOW = H_LOW_S;

% LOW PASS - MEDIUM 
M = 346;
N = 4096;
H_LOW = H_LOW_M;

% LOW PASS - LARGE
M = 693;
N = 8192;
H_LOW = H_LOW_L;

% RESAMPLING - SMALL
N_RES = N_RES_S;
H_RES = H_RES_S;

% RESAMPLING - MEDIUM 
N_RES = N_RES_M;
H_RES = H_RES_M;

% RESAMPLING - LARGE
N_RES = N_RES_L;
H_RES = H_RES_L;

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

% H_DEMOD
fprintf(fileID,'const double H_DEMOD[2][%d] = {{', N_DEMOD);
for j = 1:N_DEMOD-1
    fprintf(fileID,'%.15f, ', H_DEMOD_1(j));
end
fprintf(fileID,'%.15f},\n\t\t{', H_DEMOD_1(N_DEMOD));
for j = 1:N_DEMOD-1
    fprintf(fileID,'%.15f, ', H_DEMOD_2(j));
end
fprintf(fileID,'%.15f}};\n\n', H_DEMOD_2(N_DEMOD));

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

% OSC
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
fprintf(fileID,'#define N %d\n', N);
fprintf(fileID,'#define L %d\n', N-M);
fprintf(fileID,'#define M %d\n', M);
fprintf(fileID,'#define M_RES %d\n', N_RES);
fprintf(fileID, '\nconst double H_DEMOD[2][%d];\n', N_DEMOD);
fprintf(fileID, '\nconst double H_LOW[%d][2];\n', N);
fprintf(fileID, '\nconst double H_RES[6][%d];\n', N_RES);
fprintf(fileID, '\nconst double OSC[200];\n');
fprintf(fileID,'\n#endif\n');

fclose(fileID);
