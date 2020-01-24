file = fopen("sum_coeffs.h","w");
lines = coeff_sum; 
fprintf(file,"#ifndef _coeff\n#define _coeff\nconst float[1500]={");
for i=1:1499
    fprintf(file,"%.6f,",coeff_sum(i));
end
fprintf(file,"%.6f};\n#endif",coeff_sum(1500));
fclose(file);
    
    