close all
fn = 'test_input.dat';
fileID = fopen(fn);
a = fread(fileID, 'float32');

fs = 10e6;
%BANDPASS
sum_pass = [0.98e6,1.02e6];
diff_pass = [1.03e6, 1.07e6];
sum = bandpass(a, sum_pass, fs, 'ImpulseResponse', 'fir');
diff = bandpass(a, diff_pass, fs, 'ImpulseResponse', 'fir');
%DEMODULATE
fc_sum = 1e6;
fc_diff = 1.05e6;
sum = demod(sum, fc_sum, fs, 'amdsb-sc');
diff = demod(diff, fc_diff, fs, 'amdsb-sc');
%RESAMPLE
sum = resample(sum,3,625);
diff = resample(diff,3,625);
%LOWPASS
%sum = lowpass(sum, 20e3, fs, 'ImpulseResponse', 'fir');
%diff = lowpass(diff, 20e3, fs, 'ImpulseResponse', 'fir');
%R AND L
Rnew = (sum + diff)/2;
Lnew = (sum - diff)/2;
%RESAMPLE
%R = resample(R,3,625);
%L = resample(L,3,625);

%b = fft(sum_demod);
%L = length(sum_demod);
%P2 = abs(b/L);
%f = 20e3*(0:L-1)/L;
%figure
%plot(f, P2)

plot(Rnew)
hold on
plot(Lnew)
hold off

figure
plot(RTrue)
hold on
plot(LTrue)
hold off
