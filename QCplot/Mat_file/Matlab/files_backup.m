function []=files_backup()
us_dir  = ('E:\Code\����ͬ��ʶ�����\QT_2threads_36(WiFi)\QCplot\Mat_file');%~ �����ź��ļ�·��������λ������еĻ����Ƿ�����һ��Ŀ¼��
cd (us_dir)
file_move = dir('*.bin');%~ dir�������г��ļ��е�����
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./����');
end

file_move = dir('*.txt');
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./����');
end

sprintf('�ļ����ݳɹ�')

end
