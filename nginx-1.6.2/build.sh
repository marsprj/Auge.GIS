/opt/nginx-1.6.2/sbin/nginx -s stop
rm -rf /opt/nginx-1.6.2
./configure --prefix=/opt/nginx-1.6.2 --with-debug --add-module=/home/geobeans/code/auge.gis/ngx_http_auge_module --with-cc="gcc -g"
make
make install
/opt/nginx-1.6.2/sbin/nginx
