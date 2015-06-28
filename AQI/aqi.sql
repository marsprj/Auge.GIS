ALTER TABLE gc_aqi  ADD COLUMN x double precision;
ALTER TABLE gc_aqi  ADD COLUMN y double precision;
select addgeometrycolumn('gc_aqi','pos',4326,'POINT',2);

#鞍山,太阳城

update gc_aqi set position_name='环保监测大楼' where area='榆林' and position_name='环保监测大楼　';
update gc_aqi set position_name='市监测站' where area='焦作' and position_name='市监测站　';
update gc_aqi set position_name='文化广场' where area='牡丹江' and position_name='文化广场　';
update gc_aqi set position_name='新建' where area='七台河' and position_name='新建　';
update gc_aqi_station set station='二  印' where station='二印';
update gc_aqi set position_name='环保局' where area='南阳' and position_name='环保局　';
update gc_aqi set position_name='环保局' where area='南阳' and position_name='环保局　';
update gc_aqi set position_name='环境监测站' where area='商丘' and position_name='环境监测站　';
update gc_aqi set position_name='粮食局' where area='商丘' and position_name='粮食局　';
update gc_aqi set position_name='睢阳区环保局' where area='商丘' and position_name='睢阳区环保局';
update gc_aqi set position_name='第三人民医院' where area='商丘' and position_name='第三人民医院　';
update gc_aqi set position_name='红石峡森林公园' where area='榆林' and position_name='红石峡森林公园　';
update gc_aqi set position_name='世纪广场' where area='榆林' and position_name='世纪广场　';
update gc_aqi set position_name='环保监测大楼' where area='榆林' and position_name='环保监测大楼';
update gc_aqi set position_name='环保监测大楼' where area='焦作' and position_name='环保监测大楼　';
update gc_aqi set position_name='市环保局' where area='焦作' and position_name='市环保局　';
update gc_aqi set position_name='环保大楼' where area='牡丹江' and position_name='环保大楼　';
update gc_aqi set position_name='江南子站' where area='贵港' and position_name='江南子站　';
update gc_aqi set position_name='市一纸厂' where area='驻马店' and position_name='市一纸厂　';
update gc_aqi set position_name='市彩印厂' where area='驻马店' and position_name='市彩印厂　';
update gc_aqi set position_name='天方二分厂' where area='驻马店' and position_name='天方二分厂　';
update gc_aqi set position_name='环保局' where area='鸡西' and position_name='环保局　';
update gc_aqi set position_name='自来水公司' where area='鸡西' and position_name='自来水公司　';
update gc_aqi set position_name='白酒厂' where area='鸡西' and position_name='白酒厂　';
update gc_aqi set position_name='气象局' where area='鸡西' and position_name='气象局　';
update gc_aqi set position_name='交通局' where area='黑河' and position_name='交通局　';
update gc_aqi set position_name='地震局' where area='黑河' and position_name='地震局　';


update gc_aqi_station set station='农业园区' where city='朝阳' and station='龙城农业园区';
update gc_aqi_station set station='浑南东路' where city='沈阳' and station='浑南二';
update gc_aqi_station set station='环保大楼' where city='清远' and station='环保局';
update gc_aqi_station set station='化工街' where city='葫芦岛' and station='化工医院';
update gc_aqi_station set station='新区' where city='葫芦岛' and station='新区公园';
update gc_aqi_station set station='金沙江路北' where city='铁岭' and station='金沙江';
update gc_aqi_station set station='玉龙新城' where city='阜新' and station='玉龙新城子站';
update gc_aqi_station set station='千山' where city='鞍山' and station='千山公园';
update gc_aqi_station set station='太平' where city='鞍山' and station='太平子站';


ALTER TABLE gc_aqi  ADD COLUMN x double precision;
ALTER TABLE gc_aqi  ADD COLUMN y double precision;
select addgeometrycolumn('gc_aqi','pos',4326,'POINT',2);

insert into gc_aqi_station (city,station,x,y,pos) values('东莞','东城主山',113.793057,23.045866,st_geomfromtext('POINT(113.793057 23.045866)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('东莞','莞城梨川',113.76794,23.065702,st_geomfromtext('POINT(113.76794 23.065702)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('丹东','江湾东路',124.345766,40.072821,st_geomfromtext('POINT(124.345766 40.072821)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('丹东','临江后街',124.421763,40.151508,st_geomfromtext('POINT(124.421763 40.151508)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('丹东','元宝山',124.404165,40.156753,st_geomfromtext('POINT(124.404165 40.156753)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('丹东','民主桥',124.376605,40.119328,st_geomfromtext('POINT(124.376605 40.119328)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('克州','市人民政府',76.175804,39.722023,st_geomfromtext('POINT(76.175804 39.722023)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','南口路',117.196728,39.184062,st_geomfromtext('POINT(117.196728 39.184062)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','勤俭路',117.161011,39.176371,st_geomfromtext('POINT(117.161011 39.176371)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','大直沽八号路',117.250752,39.116534,st_geomfromtext('POINT(117.250752 39.116534)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','前进路',116.95821,39.163043,st_geomfromtext('POINT(116.95821 39.163043)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','跃进路',117.76036,39.030207,st_geomfromtext('POINT(117.76036 39.030207)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','航天路',117.412141,39.133608,st_geomfromtext('POINT(117.412141 39.133608)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('天津','汉北路',117.743436,39.122356,st_geomfromtext('POINT(117.743436 39.122356)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('广州','体育西',113.330888,23.136049,st_geomfromtext('POINT(113.330888 23.136049)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('成都','君平街',104.06325,30.661871,st_geomfromtext('POINT(104.06325 30.661871)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('抚顺','顺城区',123.943541,41.885426,st_geomfromtext('POINT(123.943541 41.885426)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('抚顺','新抚区',123.876851,41.859643,st_geomfromtext('POINT(123.876851 41.859643)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('抚顺','大伙房水库',124.165458,41.885426,st_geomfromtext('POINT(124.165458 41.885426)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('揭阳','新兴',116.125912,23.504351,st_geomfromtext('POINT(116.125912 23.504351)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('揭阳','西马',116.366226,23.514889,st_geomfromtext('POINT(116.366226 23.514889)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('揭阳','东兴',116.422442,23.559418,st_geomfromtext('POINT(116.422442 23.559418)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('揭阳','渔湖',116.420771,23.521947,st_geomfromtext('POINT(116.420771 23.521947)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('攀枝花','四十中小',101.732003,26.569892,st_geomfromtext('POINT(101.732003 26.569892)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('无锡','雪浪',120.281653,31.482659,st_geomfromtext('POINT(120.281653 31.482659)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('无锡','黄巷',120.290671,31.609601,st_geomfromtext('POINT(120.290671 31.609601)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('无锡','荣巷',120.259087,31.562445,st_geomfromtext('POINT(120.259087 31.562445)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('普洱','市环保局',100.987782,22.766061,st_geomfromtext('POINT(100.987782 22.766061)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('普洱','普洱第二中学',100.98559,22.826382,st_geomfromtext('POINT(100.98559 22.826382)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('朝阳','西梁',120.443965,41.602766,st_geomfromtext('POINT(120.443965 41.602766)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('朝阳','河畔小区',120.463297,41.574062,st_geomfromtext('POINT(120.463297 41.574062)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('朝阳','八里堡',120.425208,41.57417,st_geomfromtext('POINT(120.425208 41.57417)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('本溪','新立屯',123.811317,41.269564,st_geomfromtext('POINT(123.811317 41.269564)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','新秀街',123.436688,41.705982,st_geomfromtext('POINT(123.436688 41.705982)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','沧海路',123.302301,41.778323,st_geomfromtext('POINT(123.302301 41.778323)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','东陵路',123.528063,41.819651,st_geomfromtext('POINT(123.528063 41.819651)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','陵东街',123.446174,41.851683,st_geomfromtext('POINT(123.446174 41.851683)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','文化路',123.443156,41.77679,st_geomfromtext('POINT(123.443156 41.77679)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','南十东路',123.370501,41.790937,st_geomfromtext('POINT(123.370501 41.790937)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','京沈街',123.401977,41.935504,st_geomfromtext('POINT(123.401977 41.935504)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','森林路',123.68038,41.930138,st_geomfromtext('POINT(123.68038 41.930138)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('河源','源西',114.689752,23.755562,st_geomfromtext('POINT(114.689752 23.755562)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('河源','东埔',114.720825,23.756373,st_geomfromtext('POINT(114.720825 23.756373)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('河源','老城',113.0753,23.631941,st_geomfromtext('POINT(113.0753 23.631941)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('清远','高新区',113.065598,23.670857,st_geomfromtext('POINT(113.065598 23.670857)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('盘锦','新生街道',121.941216,41.164918,st_geomfromtext('POINT(121.941216 41.164918)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('眉山','蟆颐观',103.883047,30.057266,st_geomfromtext('POINT(103.883047 30.057266)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('营口','学府南路',122.249802,40.682857,st_geomfromtext('POINT(122.249802 40.682857)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('营口','大庆路',122.282087,40.715607,st_geomfromtext('POINT(122.282087 40.715607)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('营口','镜湖西路',122.223275,40.664068,st_geomfromtext('POINT(122.223275 40.664068)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('葫芦岛','龙港区',120.900969,40.740695,st_geomfromtext('POINT(120.900969 40.740695)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('葫芦岛','东城区',120.875816,40.727465,st_geomfromtext('POINT(120.875816 40.727465)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('襄阳','襄城运动路',112.164804,32.021349,st_geomfromtext('POINT(112.164804 32.021349)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('襄阳','樊城新华路',112.153863,32.059296,st_geomfromtext('POINT(112.153863 32.059296)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('襄阳','高新管委会',112.195778,32.118399,st_geomfromtext('POINT(112.195778 32.118399)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('襄阳','襄州航空路',112.200817,32.090458,st_geomfromtext('POINT(112.200817 32.090458)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('贵阳','燕子冲',106.761973,26.636273,st_geomfromtext('POINT(106.761973 26.636273)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('贵阳','碧云窝',106.669043,26.439547,st_geomfromtext('POINT(106.669043 26.439547)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('贵阳','中院村',106.702406,26.521532,st_geomfromtext('POINT(106.702406 26.521532)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('贵阳','红边门',106.721019,26.597651,st_geomfromtext('POINT(106.721019 26.597651)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('贵阳','新华路',106.724612,26.573646,st_geomfromtext('POINT(106.724612 26.573646)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('贵阳','鉴湖路',106.632051,26.633181,st_geomfromtext('POINT(106.632051 26.633181)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('辽阳','宏伟区',123.208949,41.219063,st_geomfromtext('POINT(123.208949 41.219063)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('辽阳','新华园',123.190876,41.265036,st_geomfromtext('POINT(123.190876 41.265036)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('辽阳','滨河路',123.183545,41.280705,st_geomfromtext('POINT(123.183545 41.280705)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('辽阳','铁西工业园',123.300397,41.441977,st_geomfromtext('POINT(123.300397 41.441977)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('铁岭','汇工街西',123.840171,42.301844,st_geomfromtext('POINT(123.840171 42.301844)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('铁岭','银州路东段',123.861052,42.295565,st_geomfromtext('POINT(123.861052 42.295565)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('锦州','百股街道',121.210359,41.126771,st_geomfromtext('POINT(121.210359 41.126771)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('锦州','天安街道',121.127275,41.128076,st_geomfromtext('POINT(121.127275 41.128076)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('长治','德盛苑',113.389181,36.510271,st_geomfromtext('POINT(113.389181 36.510271)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('阜新','长青街',121.671576,42.023853,st_geomfromtext('POINT(121.671576 42.023853)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('阜新','东苑',121.68024,42.03626,st_geomfromtext('POINT(121.68024 42.03626)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('鞍山','铁西三道街',122.984652,41.116174,st_geomfromtext('POINT(122.984652 41.116174)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('鞍山','铁西工业园区',122.946959,41.095626,st_geomfromtext('POINT(122.946959 41.095626)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('商丘','睢阳区环保局',115.65608,34.427034,st_geomfromtext('POINT(115.65608 34.427034)',4326));

insert into gc_aqi_station (city,station,x,y,pos) values('沈阳','浑南东路',123.649786,41.812192,st_geomfromtext('POINT(123.649786 41.812192)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('葫芦岛','化工街',120.848012,40.766673,st_geomfromtext('POINT(120.848012 40.766673)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('葫芦岛','新区',120.211023,40.285091,st_geomfromtext('POINT(120.211023 40.285091)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('鞍山','千山',122.968318,41.079913,st_geomfromtext('POINT(122.968318 41.079913)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('铁岭','金沙江路北',123.732668,42.22905,st_geomfromtext('POINT(123.732668 42.22905)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('朝阳','农业园区',120.405722,41.623386,st_geomfromtext('POINT(120.405722 41.623386)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('清远','环保大厦',113.054259,23.694753,st_geomfromtext('POINT(113.054259 23.694753)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('鞍山','太阳城',123.022733,41.101302,st_geomfromtext('POINT(123.022733 41.101302)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('鞍山','太平',123.066462,41.153904,st_geomfromtext('POINT(123.066462 41.153904)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('阜新','玉龙新城',121.706356,42.066428,st_geomfromtext('POINT(121.706356 42.066428)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('伊犁哈萨克','第二水厂',81.334782,43.918744,st_geomfromtext('POINT(81.334782 43.918744)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('大兴安岭地','环保局',124.129068,50.433448,st_geomfromtext('POINT(124.129068 50.433448)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('大兴安岭地','公安局',124.144357,50.427564,st_geomfromtext('POINT(124.144357 50.427564)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('伊犁哈萨克','市环保局',81.290639,43.910977,st_geomfromtext('POINT(81.290639 43.910977)',4326));
insert into gc_aqi_station (city,station,x,y,pos) values('伊犁哈萨克','新政府片区',81.284333,43.915172,st_geomfromtext('POINT(81.284333 43.915172)',4326));

