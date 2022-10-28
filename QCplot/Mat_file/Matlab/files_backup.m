function []=files_backup()
us_dir  = ('E:\2021-2022\Upper Software\QT_2threads_36_wifi_SPC(WiFi)_1220\QCplot\Mat_file');%~ 超声信号文件路径，在上位机软件中的话，是放在上一级目录下
cd (us_dir)
file_move = dir('*.bin');%~ dir函数是列出文件夹的内容
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./备份');
end

file_move = dir('*.txt');
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./备份');
end

sprintf('文件备份成功')

end
