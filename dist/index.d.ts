declare class RNCryptor {
  api: {
    FS: {
      writeFile: (filepath: string, data: Uint8Array | string) => void;
      readFile: (filepath: string, options: { encoding: "utf8" | "binary" | "base64" }) => BlobPart[];
      unlink: (filepath: string) => void;
    };
    encrypt: (pass: string, file_plain: string, file_enc: string) => 0 | 1;
    decrypt: (pass: string, file_enc: string, file_plain: string) => 0 | 1;
  };

  initialize(options: { rncryptorWasmLocation: string }): typeof RNCryptor.api;
}

export = RNCryptor;
