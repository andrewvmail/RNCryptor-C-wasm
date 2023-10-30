FROM debian:11-slim

ENV EMSCRIPTEN=/emsdk/upstream/emscripten

RUN apt-get update \
&& apt-get install -y \
git \
wget \
python3 \
xz-utils \
build-essential

RUN git clone https://github.com/emscripten-core/emsdk.git && \
cd emsdk && \
git pull && \
./emsdk install latest && \
./emsdk activate latest && \
source ./emsdk_env.sh

RUN cd / && wget https://www.openssl.org/source/openssl-1.1.0h.tar.gz && \
tar xf openssl-1.1.0h.tar.gz && \
cd openssl-1.1.0h && \
emconfigure ./Configure linux-generic64 --prefix=$EMSCRIPTEN/system && \
sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile && \
emmake make -j 12 build_generated libssl.a libcrypto.a && \
cp -R include/openssl $EMSCRIPTEN/system/include && \
cp libcrypto.a libssl.a $EMSCRIPTEN/system/lib