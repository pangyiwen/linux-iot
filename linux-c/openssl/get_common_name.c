#include <stdio.h>
#include <stdlib.h>
#include <openssl/x509.h>
#include <openssl/asn1t.h>
#include <time.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/cms.h>
#include <openssl/err.h>

int main() {
    BIO *tbio = NULL;
    X509 *rcert = NULL;
    EVP_PKEY *rkey = NULL;
    char  common_name[256] = "";
    
    tbio = BIO_new_file("./ca.crt", "r");
    rcert = PEM_read_bio_X509(tbio, NULL, 0, NULL);

    BIO_reset(tbio);
    rkey = PEM_read_bio_PrivateKey(tbio, NULL, 0, NULL);

    X509_NAME *name = X509_get_subject_name(rcert);

    X509_NAME_get_text_by_NID(name,NID_commonName, common_name, sizeof(common_name));

    printf("%s\n", common_name);
}

//gcc get_common_name.c -o app -lssl -lcrypto