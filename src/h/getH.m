%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% SUM FILTER
% All frequency values are in MHz.
Fs = 10;  % Sampling Frequency
N    = 1499;     % Order
Fc1  = 0.98;     % First Cutoff Frequency
Fc2  = 1.02;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_SUM  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% DIFF FILTER
% All frequency values are in MHz.
Fs = 10;  % Sampling Frequency
N    = 1499;     % Order
Fc1  = 1.03;     % First Cutoff Frequency
Fc2  = 1.07;     % Second Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_DIFF  = fir1(N, [Fc1 Fc2]/(Fs/2), 'bandpass', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% RESAMPLING FILTER
% All frequency values are in kHz.
Fs = 48;  % Sampling Frequency
N    = 1499;     % Order
Fc   = 20;       % Cutoff Frequency
flag = 'scale';  % Sampling Flag
Beta = 4.54;     % Window Parameter
% Create the window vector for the design algorithm.
win = kaiser(N+1, Beta);
% Calculate the coefficients using the FIR1 function.
H_RES  = fir1(N, Fc/(Fs/2), 'low', win, flag);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.c FILE
N = 1500;
fileID = fopen('init.c','w');

% void get_H_SUM(const double *p_H_SUM)
fprintf(fileID,'void get_H_SUM(const double *p_H_SUM) {\n');
fprintf(fileID,'\t*p_H_SUM = %.15f;\n', H_SUM(1));
for i = 2:N
    fprintf(fileID,'\t*(p_H_SUM + %d) = %.15f;\n', i-1, H_SUM(i));
end
fprintf(fileID,'}\n\n');

% void get_H_DIFF(const double *p_H_DIFF)
fprintf(fileID,'void get_H_DIFF(const double *p_H_DIFF) {\n');
fprintf(fileID,'\t*p_H_DIFF = %.15f;\n', H_DIFF(1));
for i = 2:N
    fprintf(fileID,'\t*(p_H_DIFF + %d) = %.15f;\n', i-1, H_DIFF(i));
end
fprintf(fileID,'}\n\n');

% void get_H_RES(const double *p_H0, const double *p_H1, const double *p_H2)
N = N/3;
H0 = zeros(1, N);
H1 = zeros(1, N);
H2 = zeros(1, N);
for i = 1:N-1
    H0(i) = H_RES((i*3)-2);
    H1(i) = H_RES((i*3)-1);
    H2(i) = H_RES(i*3);
end
fprintf(fileID,'void get_H_RES(const double *p_H0, const double *p_H1, const double *p_H2) {\n');
% H0
fprintf(fileID,'\t*p_H0 = %.15f;\n', H0(1));
for i = 2:N
    fprintf(fileID, '\t*(p_H0 + %d) = %.15f;\n', i-1, H0(i));
end
fprintf(fileID,'\n');
% H1
fprintf(fileID,'\t*p_H1 = %.15f;\n', H1(1));
for i = 2:N
    fprintf(fileID, '\t*(p_H0 + %d) = %.15f;\n', i-1, H1(i));
end
fprintf(fileID,'\n');
% H2
fprintf(fileID,'\t*p_H0 = %.15f;\n', H2(1));
for i = 2:N
    fprintf(fileID, '\t*(p_H0 + %d) = %.15f;\n', i-1, H2(i));
end
fprintf(fileID,'}');
fclose(fileID);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.h FILE
fileID = fopen('init.h','w');
fprintf(fileID,'void get_H_SUM(const double *p_H_SUM);');
fprintf(fileID,'void get_H_DIFF(const double *p_H_DIFF);');
fprintf(fileID,'void get_H_RES(const double *p_H0, const double *p_H1, const double *p_H2);');
fclose(fileID);
