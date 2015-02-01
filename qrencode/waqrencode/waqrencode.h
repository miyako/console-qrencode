#ifndef __WAQRENCODE_H__
#define __WAQRENCODE_H__

#define OPT_MICRO                       'M'
#define OPT_KANJI                       'K'

#define OPT_OUTPUT_TYPE                 't'
#define OPT_ERROR_CORRECTION_LEVEL      'l'
#define OPT_DPI                         'd'
#define OPT_VERSION                     'v'

#define OPT_SIZE                        's'
#define OPT_MARGIN                      'm'

#define OPT_INPUT                       'i'
#define OPT_OUTPUT                      'o'

#include "qrencode.h"
#include "png.h"

#define OPT_LIST            "MKt:l:d:v:s:m:i:o:"

#define BUFFER_SIZE 8192

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

enum output_type_t {
    QR_OUTPUT_PNG,
    QR_OUTPUT_SVG,
    QR_OUTPUT_PNG_BASE64    
};

#include <string>
#include <vector>

#define INCHES_PER_METER (100.0/2.54)

void toPNG(QRcode *qr, int margin, int size, int dpi, FILE *fp, bool base64 = false);
void toSVG(QRcode *qr, int margin, int size, int dpi, FILE *fp);

void printB64(std::vector<unsigned char> *png, FILE *fp);
void write_data_fn(png_structp png_ptr, png_bytep buf, png_size_t size);
void output_flush_fn(png_structp png_ptr);

#ifndef __linux__
#ifndef __APPLE__
#define __WINDOWS__
#include <fcntl.h> 
#include <io.h>
#include "unistd.h"
#include "getopt.h"
#define strcasecmp _stricmp
void unescape_path(std::string &path);
FILE *_fopen(std::string &path, const wchar_t *mode);
#else
#endif
#endif

#ifndef __WINDOWS__
#include <unistd.h>
#include <getopt.h>
#endif

#endif