function  BrainTo1DArray(all,...
    start_X,...
    end_X,...
    start_Y,...
    end_Y,...
    start_Z,...
    end_Z)
directory_name = uigetdir;
cd(directory_name);
load brain.mat;
[dim_t,dim_x,dim_y,dim_z]=size(SM);
if(all)
    start_X=1;
    end_X=dim_x;
    start_Y=1;
    end_Y=dim_y;
    start_Z=1;
    end_Z=dim_z;
end

B=SM(1:dim_t,start_X:end_X,start_Y:end_Y,start_Z:end_Z);
dim_mat=length(start_X:end_X)*length(start_Y:end_Y)*length(start_Z:end_Z);

C=reshape(B,1,dim_t*dim_mat);
%C=reshape(BN',1,1000*132);
[file,path] = uiputfile('FMRI.bin','Save file name');
full_path=[path file];
fid=fopen(full_path,'w');
fwrite(fid,C,'int');
fclose(fid);
end