Beta = 4.54;     % Window Parameter (4.54, 6.76, 8.96)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - SMALL
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_S    = 128;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_S+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_S  = fir1(N_S, Fc/(Fs/2), 'low', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - MEDIUM
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_M  = 256;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_M+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_M  = fir1(N_M, Fc/(Fs/2), 'low', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW PASS FILTER - LARGE
% All frequency values are in MHz.
Fs = 5;  % Sampling Frequency
N_L    = 512;     % Order
Fc  = 0.025;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag
% Create the window vector for the design algorithm.
win = kaiser(N_L+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_LOW_L  = fir1(N_L, Fc/(Fs/2), 'low', win, flag);

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
% % LOW PASS - SMALL
% M = 174;
% N = N_S;
% H_LOW = H_LOW_S;
% 
% % LOW PASS - MEDIUM 
% M = 347;
% N = 4096;
% H_LOW = H_LOW_M;

% LOW PASS - LARGE
M = 693;
N = N_L;
H_LOW = H_LOW_L;
% 
% % RESAMPLING - SMALL
% N_RES = N_RES_S;
% H_RES = H_RES_S;
% 
% % RESAMPLING - MEDIUM 
% N_RES = N_RES_M;
% H_RES = H_RES_M;

% RESAMPLING - LARGE
N_RES = N_RES_L;
H_RES = H_RES_L;

fileID = fopen('init.c','w');

% H_LOW
fprintf(fileID,'const double H_LOW[%d] = {', N);
for i = 1:N-1
    fprintf(fileID,'%.15f,', H_LOW(i));
    if mod(i,8)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.15f,};\n\n', H_LOW(N));


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
fprintf(fileID,'#define N %d\n', 0);
fprintf(fileID,'#define L %d\n', 1875);
fprintf(fileID,'#define M %d\n', N);
fprintf(fileID,'#define M_RES %d\n', N_RES);
SIZE_OUT = 1875 * 6/625;
fprintf(fileID,'#define SIZE_OUT %d\n', SIZE_OUT);
fprintf(fileID, '\nconst double H_LOW[%d];\n', N);
fprintf(fileID, '\nconst double H_RES[6][%d];\n', N_RES);
fprintf(fileID, '\nconst double OSC[200];\n');
fprintf(fileID,'\n#endif\n');

fclose(fileID);