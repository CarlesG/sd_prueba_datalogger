%% Import data from text file
% Script for importing data from the following text file:
%
%    filename: E:\Users\cargall2\sd_prueba_datalogger\read_temperature\prueba_temperatura\prueba_temperatura_ds18b20_2sensores_mismobus_datalogger\TEST.csv
%
% Auto-generated by MATLAB on 07-Dec-2022 14:16:51

%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 5);

% Specify range and delimiter

opts.DataLines = [1, Inf];
opts.Delimiter = ";";

% Specify column names and types
% opts.VariableNames = ["date", "time", "temp1", "temp2", "temp3","voltage"];
% opts.VariableTypes = ["string", "string", "double", "double", "double","double"];
opts.VariableNames = ["date", "time", "temp1", "temp2", "temp3"];
opts.VariableTypes = ["string", "string", "double", "double", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, ["date", "time"], "WhitespaceRule", "preserve");
opts = setvaropts(opts, ["date", "time"], "EmptyFieldRule", "auto");

% Import the data
data = readtable("E:\Users\cargall2\sd_prueba_datalogger\read_temperature\prueba_temperatura\prueba_temperatura_ds18b20_2sensores_mismobus_datalogger\TEST.csv", opts);

time_stamp_string = strcat(data.date, data.time);
time_stamp = datetime(time_stamp_string, 'InputFormat', 'ddMMyyyyHHmmss');
time_stamp_represent = hours(time_stamp - time_stamp(1));
t1 = data.temp1;
t2 = data.temp2;
t3 = data.temp3;
figure(1),plot(time_stamp_represent, t1, 'r.', time_stamp_represent, t2, 'b.', time_stamp_represent, t3, 'g.')
legend("red temperature sensor", " blue temperature sensor 2", "ambient temperature sensor")
xlabel('Hours (h)')
ylabel("Temperature (ºC)")
%% Clear temporary variables
clear opts