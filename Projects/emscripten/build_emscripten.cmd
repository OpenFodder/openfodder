copy openfodder.ini ..\..\Run
docker run --rm -v %cd%\..\..\:/src -t apiaryio/emcc make -f EMakefile

docker stop openfodder-web
docker run --rm -v %cd%\..\..\RunE:/usr/local/apache2/htdocs/ -v %cd%\httpd.conf:/usr/local/apache2/conf/httpd.conf -dit --name openfodder-web -p 8080:80 httpd:2.4

@pause