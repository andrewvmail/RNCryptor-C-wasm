
### Installation
```bash
npm install rncryptor-c-wasm 
cp -r node_modules/rncryptor-c-wasm/dist/rncryptor.wasm public/
```

### Usage (vite)
```js
import rncryptor from "rncryptor-c-wasm";
import rncryptorWasm from "/rncryptor.wasm?url&file"; 

rncryptor({
  locateFile: (f) => {
    return rncryptorWasm;
  },
}).then(async (instance) => {
  console.log("instance", instance);
  const api = instance;
  const { FS } = instance;
  var data = "Hello, world!";
  // var data = new Uint8Array([1, 2, 3, 4, 5]);
  instance.FS.writeFile("/myfile.plain", data);
  // run encryption
  console.log(
    "return code encrypt:",
    api._encrypt("mypass", "/myfile.plain", "/myfile.enc"),
  );

  // read the encrypted file
  var binary = FS.readFile("/myfile.plain", { encoding: "utf8" });
  var blob = new Blob([binary], { type: "application/octet-stream" });

  // run encryption
  console.log(
    "return code decrypt:",
    api._decrypt("mypass", "/myfile.enc", "myfile.plain"),
  );

  console.log("text from decrypt", await blob.text());
});
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
source /emsdk/emsdk_env.sh && mkdir -p dist/ && EMCC_DEBUG=1 emcc /RNCryptor-C-wasm/src/rncryptor.c /RNCryptor-C-wasm/RNCryptor-C/rncryptor_c.c /RNCryptor-C-wasm/RNCryptor-C/mutils.c -I RNCryptor-C $EMSCRIPTEN/system/lib/libssl.a $EMSCRIPTEN/system/lib/libcrypto.a -I $EMSCRIPTEN/cache/sysroot/include -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s EXPORTED_RUNTIME_METHODS='["cwrap", "FS"]' -s ENVIRONMENT="web" -s ALLOW_MEMORY_GROWTH=1 -s EXPORT_ES6=1 -s MODULARIZE=1 -s WASM_ASYNC_COMPILATION=1 -o dist/rncryptor.js
```