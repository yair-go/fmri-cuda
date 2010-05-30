function C=Brain_TO_1D_Array(SM) 

B=SM(1:132,41:50,41:48,41:56);
C=reshape(B,1,132*1280);
%C=reshape(BN',1,1000*132);
fid=fopen('c:\Vis\FMRI.bin','w');
fwrite(fid,C,'int');
fclose(fid)
end