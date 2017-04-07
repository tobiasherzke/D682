function [L, standard] = nhthresholds( f, standard )
  s = threshold_db;
  stds = fieldnames(s)';
  if nargin < 1
    L = stds;
    return;
  end
  if nargin < 2
    standard = stds{1};
  end
  if isempty(strmatch(standard,stds,'exact'))
    sstds = '';
    for s=stds
      sstds = [sstds,' ',s{:}];
    end
    error(sprintf(['Unknown standard "%s" (known standards:' ...
				      '%s)'],standard,sstds));
  end
  vData = s.(standard);
  L = interp1(log(vData(:,1)),vData(:,2),log(f),'linear','extrap');

function thr = threshold_db
  thr.iso226_1985 = ...
      [20   74.3;...
       25   65.0;...
       31.5   56.3;...
       40   48.4;...
       50   41.7;...
       63   35.5;...
       80   29.8;...
       100   25.1;...
       125   20.7;...
       160   16.8;...
       200   13.8;...
       250   11.2;...
       315    8.9;...
       400    7.2;...
       500    6.0;...
       630    5.0;...
       800    4.4;...
       1000    4.2;...
       1250    3.8;...
       1600    2.6;...
       2000    1.0;...
       2500   -1.2;...
       3150   -3.6;...
       4000   -3.9;...
       5000   -1.1;...
       6300    6.6;...
       8000   15.3;...
       10000   16.4;...
       12500   11.6;...
       14000   16.0;...
       15000   24.1];

  thr.iso226_1987 = ...
      [20   74.3;...
       25   65.0;...
       31.5 56.3;...
       40   48.4;...
       50   41.7;...
       63   35.5;...
       80   29.8;...
       100  25.1;...
       125  20.7;...
       160  16.8;...
       200  13.8;...
       250  11.2;...
       315   8.9;...
       400   7.2;...
       500   6.0;...
       630   5.0;...
       800   4.4;...
       1000  4.2;...
       1250  3.7;...
       1600  2.6;...
       2000  1.0;...
       2500  -1.2;...
       3150  -3.6;...
       4000  -3.9;...
       5000  -1.1;...
       6300  6.6;...
       8000 15.3;...
       10000 16.4;...
       12500 11.6];

  thr.iso389_7_2004 = ...
      [20 78.5   ;...
       25 68.7   ;...
       31.5 59.5 ;...
       40 51.1   ;...
       50 44.0   ;...
       63 37.5   ;...
       80 31.5   ;...
       100 26.5  ;...
       125 22.1  ;...
       160 17.9  ;...
       200 14.4  ;...
       250 11.4  ;...
       315 8.6   ;...
       400 6.2   ;...
       500 4.4   ;...
       630 3.0   ;...
       750 2.4   ;...
       800 2.2   ;...
       1000 2.4  ;...
       1250 3.5  ;...
       1500 2.4  ;...
       1600 1.7  ;...
       2000 -1.3 ;...
       2500 -4.2 ;...
       3000 -5.8 ;...
       3150 -6.0 ;...
       4000 -5.4 ;...
       5000 -1.5 ;...
       6000 4.3  ;...
       6300 6.0  ;...
       8000 12.6 ;...
       9000 13.9 ;...
       10000 13.9 ;...
       11200 13.0 ;...
       12500 12.3 ;...
       14000 18.4 ;...
       16000 40.2 ;...
       18000 73.2];
