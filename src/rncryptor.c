#include <stdio.h>
#include <openssl/evp.h>
#include "/RNCryptor-C-wasm/RNCryptor-C/rncryptor_c.h"
#include <string.h>
#include "/emsdk/upstream/emscripten/cache/sysroot/include/emscripten.h"

EMSCRIPTEN_KEEPALIVE
int encrypt(
    const char *password, 
    const char *file_enc,
    const char *file_plain) {

    printf("The string is: %s\n", password);
    if (password == NULL || file_plain == NULL || file_enc == NULL) {
        printf("Error: password, file_plain & file_enc parameters must be provided.\n");
        return(1);
    }
    // Open the file
    FILE *file = fopen(file_plain, "rb");
    if (file == NULL) {
        printf("Failed to open the file\n");
        return(1);
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Print the file size
    printf("The file size is: %ld bytes\n", file_size);

    // Close the file
    fclose(file);

    int
    rc;

    char
    errbuf[BUFSIZ];

    unsigned char
    *outdata = NULL;

    int 
    outdata_len = 0;

    memset(errbuf,0,sizeof(errbuf));
    rncryptorc_set_debug(0);
    outdata = rncryptorc_encrypt_file_with_password(file_plain,
        RNCRYPTOR3_KDF_ITER,
        password,strlen(password),
        &outdata_len,
        errbuf,
        sizeof(errbuf)-1);
    if (outdata)
    {
        rc = rncryptorc_write_file(file_enc,outdata,outdata_len);
        (void)free((char *)outdata);
        if (rc == SUCCESS)
        {
            (void) fprintf(stdout,"Encrypted file: %s\n",file_enc);
            return(0);
        }
    }
    else
    {
        (void) fprintf(stderr,"ERROR: %s\n",errbuf);
        return(1);
    }

    return(0);
}

EMSCRIPTEN_KEEPALIVE
int decrypt(
    const char *password, 
    const char *file_plain,
    const char *file_enc) {


    if (password == NULL || file_plain == NULL || file_enc == NULL) {
        printf("Error: password, file_plain & file_enc parameters must be provided.\n");
        return(1);
    }

    unsigned char
    *outdata = NULL;

    int
    outdata_len = 0,
    rc;

    char
    errbuf[BUFSIZ];

    memset(errbuf,0,sizeof(errbuf));
    rncryptorc_set_debug(0);
    outdata = rncryptorc_decrypt_file_with_password(file_enc,
        RNCRYPTOR3_KDF_ITER,
        password,strlen(password),
        &outdata_len,
        errbuf,sizeof(errbuf)-1);
    if (outdata)
    {
        rc = rncryptorc_write_file(file_plain,outdata,outdata_len);
        (void) free((char *)outdata);
        if (rc == SUCCESS)
        {
            if (*file_plain != '-')
                (void) fprintf(stdout,"%s:%d - Decrypted to %s\n",MCFL,file_plain);
            else
            {
                (void) fflush(stdout);
            }
            return(0);
        }
    }

    return(1);
}
