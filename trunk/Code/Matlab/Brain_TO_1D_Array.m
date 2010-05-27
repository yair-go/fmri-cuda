function C=Brain_TO_1D_Array(SM) 

B=SM(1:132,41:50,41:50,41:50);
C=reshape(B,1,132*1000);
%C=reshape(BN',1,1000*132);
fid=fopen('c:\Vis\FMRI.bin','w');
fwrite(fid,C,'int');
fclose(fid)
end