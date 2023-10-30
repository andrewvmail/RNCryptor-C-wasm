FROM debian:11-slim

SHELL ["/bin/bash", "-c"]   

ENV EMSCRIPTEN=/emsdk/upstream/emscripten
VOLUME /RNCryptor-C-wasm

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

RUN source /emsdk/emsdk_env.sh \
&& cd / && wget https://www.openssl.org/source/openssl-1.1.0h.tar.gz && \
tar xf openssl-1.1.0h.tar.gz && \
cd openssl-1.1.0h && \
emconfigure ./Configure linux-generic64 --prefix=$EMSCRIPTEN/system && \
sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile && \
emmake make -j 12 build_generated libssl.a libcrypto.a && \
cp -R include/openssl $EMSCRIPTEN/system/include && \
cp libcrypto.a libssl.a $EMSCRIPTEN/system/lib

# COPY . /RNCryptor-C-wasm
WORKDIR /RNCryptor-C-wasm
RUN EMCC_DEBUG=1 emcc src/rncryptor.c $EMSCRIPTEN/system/lib/libssl.a $EMSCRIPTEN/system/lib/libcrypto.a -I $EMSCRIPTEN/cache/sysroot/include -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s EXPORTED_RUNTIME_METHODS='["cwrap"]'