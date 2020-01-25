%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% SUM FILTER
% All frequency values are in MHz.
Fs = 10;  % Sampling Frequency
N    = 1499;     % Order
Fc1  = 0.98;     % First Cutoff Frequency
Fc2  = 1.03;     % Second Cutoff Frequency
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

% float * get_H_SUM()
fprintf(fileID,'float * get_H_SUM() {\n');
fprintf(fileID,'\tconst float H_SUM[%d] = {\n\t\t', N);
for i = 1:N-1
    fprintf(fileID,'%.6f,', H_SUM(i));
    if mod(i,15)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.6f};\n', H_SUM(N));
fprintf(fileID,'\treturn H_SUM;\n}\n\n');

% float * get_H_DIFF()
fprintf(fileID,'float * get_H_DIFF() {\n');
fprintf(fileID,'\tconst float H_DIFF[%d] = {\n\t\t', N);
for i = 1:N-1
    fprintf(fileID,'%.6f,', H_DIFF(i));
    if mod(i,15)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.6f};\n', H_DIFF(N));
fprintf(fileID,'\treturn H_DIFF;\n}\n\n');

% void get_H_RES(float *p_H0, float *p_H1, float *p_H2)
N = N/3;
H0 = zeros(1, N);
H1 = zeros(1, N);
H2 = zeros(1, N);
for i = 1:N-1
    H0(i) = H_RES((i*3)-2);
    H1(i) = H_RES((i*3)-1);
    H2(i) = H_RES(i*3);
end
fprintf(fileID,'void get_H_RES(float *p_H0, float *p_H1, float *p_H2) {\n');
% H0
fprintf(fileID,'\tconst float H0[%d] = {\n\t\t', N);
for i = 1:N-1
    fprintf(fileID,'%.6f,', H0(i));
    if mod(i,15)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.6f};\n', H0(N));
fprintf(fileID,'\tp_H0 = H0;\n');
% H1
fprintf(fileID,'\tconst float H1[%d] = {\n\t\t', N);
for i = 1:N-1
    fprintf(fileID,'%.6f,', H1(i));
    if mod(i,15)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.6f};\n', H1(N));
fprintf(fileID,'\tp_H1 = H1;\n');
% H2
fprintf(fileID,'\tconst float H2[%d] = {\n\t\t', N);
for i = 1:N-1
    fprintf(fileID,'%.6f,', H2(i));
    if mod(i,15)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.6f};\n', H2(N));
fprintf(fileID,'\tp_H2 = H2;\n}');
fclose(fileID);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.h FILE
fileID = fopen('init.h','w');
fprintf(fileID,'float * get_H_SUM();\n');
fprintf(fileID,'float * get_H_DIFF();\n');
fprintf(fileID,'void get_H_RES(float *p_H0, float *p_H1, float *p_H2);');
fclose(fileID);
