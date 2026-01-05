@echo off
set ROOT=%cd%\..\..
set BUILD_VOL=openfodder-ems-build
set CACHE_VOL=openfodder-ems-cache
set TOOLS_VOL=openfodder-ems-tools

docker volume create %BUILD_VOL% >nul 2>&1
docker volume create %CACHE_VOL% >nul 2>&1
docker volume create %TOOLS_VOL% >nul 2>&1

copy openfodder.ini ..\..\Run
docker run --rm -v %ROOT%:/src -v %BUILD_VOL%:/build -v %CACHE_VOL%:/emsdk_cache -v %TOOLS_VOL%:/tools -e EM_CACHE=/emsdk_cache -t emscripten/emsdk:latest /bin/bash -lc "set -e; export EM_CACHE=/emsdk_cache; if [ ! -x /tools/ninja ]; then curl -L -o /tools/ninja-linux.zip https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip; cd /tools; unzip -o ninja-linux.zip; chmod +x /tools/ninja; fi; export PATH=/tools:$PATH; emcmake cmake -S /src -B /build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/tools/ninja; /tools/ninja -C /build -v"

docker stop openfodder-web
docker run --rm -v %cd%\..\..\RunE:/usr/local/apache2/htdocs/ -v %cd%\httpd.conf:/usr/local/apache2/conf/httpd.conf -dit --name openfodder-web -p 8080:80 httpd:2.4

@pause
