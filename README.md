```
docker build . -t wasmbuilder
docker run -v "$(pwd)":/RNCryptor-C-wasm -it wasmbuilder /bin/bash
```