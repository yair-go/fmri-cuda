function C=Brain_TO_1D_Array(SM) 

B=SM(1:132,41:50,41:50,41:50);
BN=reshape(B,1000,132);
C=reshape(BN',1,1000*132);
fid=fopen('c:\FMR.bin','w');
fwrite(fid,C);
flcose(fid)
end