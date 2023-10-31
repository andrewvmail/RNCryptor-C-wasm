
### Installation
```bash
npm install rncryptor-c-wasm 
cp -r node_modules/rncryptor-c-wasm/dist/rncryptor.wasm public/
```

### Usage (vite)
```js
import Rncryptor from "rncryptor-c-wasm";
import rncryptorWasmLocation from "/rncryptor.wasm?url&file";

const rncryptor = await Rncryptor.initialize({
  rncryptorWasmLocation,
});
const data = "Hello, world!";
// const data = new Uint8Array([1, 2, 3, 4, 5]);
rncryptor.api.FS.writeFile("/myfile.plain", data);
rncryptor.api.encrypt("mypass", "/myfile.plain", "/myfile.enc");

// delete the myfile.plain from the virtual filesystem
rncryptor.api.FS.unlink("/myfile.plain");

rncryptor.api.decrypt("mypass", "/myfile.enc", "myfile.plain");
const blobParts = rncryptor.api.FS.readFile("/myfile.plain", {
  encoding: "utf8",
});
const blob = new Blob([blobParts], {
  type: "application/octet-stream",
});
console.log("text from decrypt", await blob.text());

rncryptor.api.FS.unlink("/myfile.plain");
rncryptor.api.FS.unlink("/myfile.enc");
```


### Building locally
```bash
git clone https://github.com/andrewvmail/RNCryptor-C-wasm
git submodule init
git submodule update
(cd RNCryptro-C && git patch include-fix.patch)
docker build . -t wasmbuilder
docker run -v "$(pwd)":/RNCryptor-C-wasm -it wasmbuilder /bin/bash 

# inside the container
source /emsdk/emsdk_env.sh && mkdir -p dist/ && \
EMCC_DEBUG=1 emcc \
/RNCryptor-C-wasm/src/rncryptor.c \
/RNCryptor-C-wasm/RNCryptor-C/rncryptor_c.c \
/RNCryptor-C-wasm/RNCryptor-C/mutils.c \
-I RNCryptor-C $EMSCRIPTEN/system/lib/libssl.a $EMSCRIPTEN/system/lib/libcrypto.a \
-I $EMSCRIPTEN/cache/sysroot/include \
-s ERROR_ON_UNDEFINED_SYMBOLS=0 \
-s EXPORTED_RUNTIME_METHODS='["cwrap", "FS"]' \
-s ENVIRONMENT="web" \
-s ALLOW_MEMORY_GROWTH=1 \
-s EXPORT_ES6=1 \
-s MODULARIZE=1 \
-s WASM_ASYNC_COMPILATION=1 \
-o dist/rncryptor.js
```