%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CHOOSE FILTERS

% Choose from: 16, 32, 64, 128, 256, 512
N_LOW_1 = 32;
N_LOW_2 = 128;
N_RES =   256;
Beta = 4.54;     % Window Parameter (4.54, 6.76, 8.96)

H_LOW_1 = getH(5, (N_LOW_1-1), 2, Beta);
H_LOW_2 = getH(0.2, (N_LOW_2-1), 0.05, Beta);
H_RES = getH(1.2, ((N_RES*6)-1), 0.025, Beta);
H_RES = reshape(H_RES,[6,N_RES]);

% Batch Size * 6/625 = int
L = 1875;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.c FILE
fileID = fopen('init.c','w');

% H_LOW_1
fprintf(fileID,'const double H_LOW_1[%d] = {', N_LOW_1);
for i = 1:N_LOW_1-1
    fprintf(fileID,'%.15f,', H_LOW_1(i));
    if mod(i,8)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.15f};\n\n', H_LOW_1(N_LOW_1));

% H_LOW_2
fprintf(fileID,'const double H_LOW_2[%d] = {', N_LOW_2);
for i = 1:N_LOW_2-1
    fprintf(fileID,'%.15f,', H_LOW_2(i));
    if mod(i,8)==0
        fprintf(fileID,'\n\t\t');
    end
end
fprintf(fileID,'%.15f};\n\n', H_LOW_2(N_LOW_2));

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
        fprintf(fileID,'%.15f}};', H_RES(i, N_RES));
    else
        fprintf(fileID,'%.15f},\n\t\t', H_RES(i, N_RES));
    end
end

fclose(fileID);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE init.h FILE
fileID = fopen('init.h','w');

fprintf(fileID,'#ifndef _CONSTS\n');
fprintf(fileID,'#define _CONSTS\n\n');
fprintf(fileID,'#define L %d\n', L);
fprintf(fileID, '#define L2 %d\n', (L*2));
fprintf(fileID, '#define L_2 %d\n', (L/2));
fprintf(fileID,'#define M1 %d\n', N_LOW_1);
fprintf(fileID,'#define M2 %d\n', N_LOW_2);
fprintf(fileID,'#define M_RES %d\n', N_RES);
fprintf(fileID,'#define SIZE_OUT %d\n', (L*6/625));
fprintf(fileID, '\nconst double H_LOW_1[M1];\n');
fprintf(fileID, '\nconst double H_LOW_2[M2];\n');
fprintf(fileID, '\nconst double H_RES[6][M_RES];\n');
fprintf(fileID,'\n#endif\n');

fclose(fileID);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% CREATE FILTERS
function H = getH(Fs, N, Fc, Beta)
	% All frequency values are in MHz.
	% Create the window vector for the design algorithm.
	win = kaiser(N+1, Beta);
	% Calculate the coefficients using the FIR1 function.
	H  = fir1(N, Fc/(Fs/2), 'low', win, 'scale');
end