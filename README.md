```
git clone https://github.com/andrewvmail/RNCryptor-C-wasm
git submodule init
git submodule update
(cd RNCryptro-C && git patch include-fix.patch)
docker build . -t wasmbuilder
docker run -v "$(pwd)":/RNCryptor-C-wasm -it wasmbuilder /bin/bash
```