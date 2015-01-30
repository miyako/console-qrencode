#include "waqrencode.h"

#include <string>

extern char *optarg;
extern int optind, opterr;

static void usage(void){
    
    fprintf(stderr, "usage: waqrencode [stdin] [stdout]\n");

    fprintf(stderr, " -%c: %s\n", OPT_MICRO , "micro qr code");  
    fprintf(stderr, " -%c: %s\n", OPT_KANJI , "kanji mode");  
    fprintf(stderr, " -%c: %s\n", OPT_OUTPUT_TYPE , "output type={png, svg}");  
    fprintf(stderr, " -%c: %s\n", OPT_ERROR_CORRECTION_LEVEL , "error correction level={LMQH}");      
    fprintf(stderr, " -%c: %s\n", OPT_DPI , "DPI for png={NUMBER} default 72");  
    fprintf(stderr, " -%c: %s\n", OPT_VERSION , "version={NUMBER}, 1<=40 (qr), 1<=4 (micro qr)");  
    fprintf(stderr, " -%c: %s\n", OPT_SIZE , "pixel size of squares={NUMBER} default 3");  
    fprintf(stderr, " -%c: %s\n", OPT_MARGIN , "pixel size of squares={NUMBER} default 0");
    fprintf(stderr, " -%c: %s\n", OPT_INPUT , "input path");  
    fprintf(stderr, " -%c: %s\n", OPT_OUTPUT , "output path"); 
    
    exit(1);
}

static FILE *getStdOut(std::string &output){

#ifdef __WINDOWS__    
	//otherwise, by default, LF will be replaced by CRLF
   _setmode(_fileno(stdout), _O_BINARY);        
#endif   

    static FILE *fp = NULL;
    
    if(output.length()){
#ifdef __WINDOWS__ 
        fp = _fopen(output, L"wb");
#else
        fp = fopen(output.c_str(), "wb");    
#endif    
    }
    
    if(!fp){
        fp = stdout;
    }    
    return fp;
}

char *getStdIn(std::string input, unsigned int *buf_size){

#ifdef __WINDOWS__  
	//otherwise, by default, LF will be replaced by CRLF
    _setmode(_fileno(stdin), _O_BINARY);        
#endif    
    
    bool isReady = false;
    
    unsigned int size = BUFFER_SIZE;
    
    char *buf = (char *)calloc(size, sizeof(char));
    
    if(!buf){
        exit(1);
    }    
    
    unsigned int ret, i;
    
    if(input.length()){
        
#ifdef __WINDOWS__ 
        FILE *fp = _fopen(input, L"rb");
#else
        FILE *fp = fopen(input.c_str(), "rb");    
#endif            
        
        if(fp){
            
            i = 0;
            
            while((ret = (unsigned int)fread(&buf[i], sizeof(char), BUFFER_SIZE, fp)) > 0){
                i += ret;
                size += BUFFER_SIZE;
                buf = (char *)realloc(buf, size);
                if(!buf){
                    exit(1);
                }
            }   
            fclose(fp);
            isReady = true;
        }
    }
    
    if(!isReady){
        
        i = 0;
        
        while((ret = (unsigned int)fread(&buf[i], sizeof(char), BUFFER_SIZE, stdin)) > 0){
            i += ret;
            size += BUFFER_SIZE;
            buf = (char *)realloc(buf, size);
            if(!buf){
                exit(1);
            }
        }
        isReady = true;
    } 
    
    *buf_size = i;
    
    return buf;
}

int __cdecl main(int argc, char *argv[])
{
	int micro = 0;
	int version = 1;
	int dpi = 72;
	int size = 3;
	int margin = 0;    
    
	output_type_t type = QR_OUTPUT_PNG;
    QRencodeMode mode = QR_MODE_8;
	QRecLevel level = QR_ECLEVEL_L;

    std::string input, output;
    
    if(argc == 1) usage();
    
    int ch;
    
    while ((ch = getopt(argc, argv, OPT_LIST)) != -1){
        
        switch (ch){
            case OPT_MICRO:
                micro = 1;
                break;
                
            case OPT_KANJI:
                mode = QR_MODE_KANJI;
                break;

            case OPT_OUTPUT_TYPE:
                if(!strcasecmp(optarg, "png")) {
                    type = QR_OUTPUT_PNG;
                } else if(!strcasecmp(optarg, "svg")) {
                    type = QR_OUTPUT_SVG;
                } else{
                    usage();
                }                      
                break;
                
            case OPT_ERROR_CORRECTION_LEVEL:
                switch(*optarg){
                    case 'l':
                    case 'L':
                    case '0':                        
                        level = QR_ECLEVEL_L;
                        break;
                    case 'm':
                    case 'M':
                    case '1':                        
                        level = QR_ECLEVEL_M;
                        break;
                    case 'q':
                    case 'Q':
                    case '2':                        
                        level = QR_ECLEVEL_Q;
                        break;
                    case 'h':
                    case 'H':
                    case '3':                        
                        level = QR_ECLEVEL_H;
                        break;
                    default:
                        usage();
                        break;
                }
                break;                

            case OPT_DPI:
                dpi = atoi(optarg);
                break;      

            case OPT_VERSION:
                version = atoi(optarg);
                break;  

            case OPT_SIZE:
                size = atoi(optarg);
                break;  

            case OPT_MARGIN:
                margin = atoi(optarg);
                break;  
                
            case OPT_INPUT:               
                input = std::string(optarg);        
                break;
                
            case OPT_OUTPUT:
                output = std::string(optarg);              
                break;                
                
            default:
                usage();
        }
    } 

    if((margin < 0) || (size <= 0) || (dpi < 0) || (version < 0)){
        usage();
    }

	unsigned int len;

    char *buf = getStdIn(input, &len);
    
    if(buf){
    
        QRcode *qr = NULL;
        
        if(micro){
        
            if(mode == QR_MODE_KANJI){
            
                qr = QRcode_encodeStringMQR((const char *)&buf[0],
                                            version,
                                            level,
                                            mode, 
                                            1);
                
            }else{
                
                qr = QRcode_encodeDataMQR((int)len, 
                                          (const unsigned char *)&buf[0], 
                                          version, 
                                          level);
            }
            
        }else{
            
            if(mode == QR_MODE_KANJI){
            
                qr = QRcode_encodeString((const char *)&buf[0],
                                         version,
                                         level,
                                         mode, 
                                         1);
            
            }else{
            
                qr = QRcode_encodeData((int)len, 
                                       (const unsigned char *)&buf[0], 
                                       version, 
                                       level);
        
            }
  
        }

        static FILE *out = getStdOut(output);
        
        if(qr){
        
            switch(type){
                case QR_OUTPUT_PNG:
                    toPNG(qr, margin, size, dpi, out);
                    break;
                    
                case QR_OUTPUT_SVG:
                    toSVG(qr, margin, size, dpi, out);
                    break;           
            }  

			QRcode_free(qr);

        }

        free(buf);
        
        fclose(out);
    }
    
    return 0;
}

void toSVG(QRcode *qr, int margin, int size, int dpi, FILE *fp)
{
	fprintf( fp, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n");
	fprintf( fp, "<svg width=\"100%%\" height=\"100%%\" viewBox=\"0 0 %d %d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:ns4d=\"http://www.4d.com\" ns4d:DPI=\"%d\">\n", margin * 2, margin * 2, dpi);
	fprintf( fp, "<g shape-rendering=\"crispEdges\" stroke-width=\"0\" stroke=\"none\" transform=\"translate(%d,%d)\">\n", margin, margin);

	unsigned char *p;
	int x, y = 0, i, j;	
	p = qr->data;
		
	for(i = 0; i < qr->width; ++i) 
	{
		x = 0;
		for(j = 0; j < qr->width; ++j) 
		{
			if(*p&1)
			{
				fprintf( fp, "<rect fill=\"black\" height=\"%d\" width=\"%d\" x=\"%d\" y=\"%d\" />\n", size, size, x, y);
			}else{
				fprintf( fp, "<rect fill=\"white\" height=\"%d\" width=\"%d\" x=\"%d\" y=\"%d\" />\n", size, size, x, y);
			}		
			p++; 
			x = x + size;
		}
		y = y + size;
	}		

	fprintf( fp, "</g>\n");
	fprintf( fp, "</svg>\n");
}
void toPNG(QRcode *qr, int margin, int size, int dpi, FILE *fp)
{
	unsigned int fg_color[4] = {0, 0, 0, 255};
	unsigned int bg_color[4] = {255, 255, 255, 255};

	png_structp png_ptr;
	png_infop info_ptr;
	png_colorp palette;
	png_byte alpha_values[2];
	unsigned char *row, *p, *q;
	int x, y, xx, yy, bit;
	int realwidth;

	realwidth = (qr->width + margin * 2) * size;
	row = (unsigned char *)malloc((realwidth + 7) / 8);
	if(row == NULL) {
		exit(1);
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		exit(1);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		exit(1);
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		exit(1);
	}

	palette = (png_colorp) malloc(sizeof(png_color) * 2);
	if(palette == NULL) {
		exit(1);
	}
	palette[0].red   = fg_color[0];
	palette[0].green = fg_color[1];
	palette[0].blue  = fg_color[2];
	palette[1].red   = bg_color[0];
	palette[1].green = bg_color[1];
	palette[1].blue  = bg_color[2];
	alpha_values[0] = fg_color[3];
	alpha_values[1] = bg_color[3];
	png_set_PLTE(png_ptr, info_ptr, palette, 2);
	png_set_tRNS(png_ptr, info_ptr, alpha_values, 2, NULL);

	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr,
			realwidth, realwidth,
			1,
			PNG_COLOR_TYPE_PALETTE,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
	png_set_pHYs(png_ptr, info_ptr,
			dpi * INCHES_PER_METER,
			dpi * INCHES_PER_METER,
			PNG_RESOLUTION_METER);
	png_write_info(png_ptr, info_ptr);

	/* top margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	/* data */
	p = qr->data;
	for(y=0; y<qr->width; y++) {
		bit = 7;
		memset(row, 0xff, (realwidth + 7) / 8);
		q = row;
		q += margin * size / 8;
		bit = 7 - (margin * size % 8);
		for(x=0; x<qr->width; x++) {
			for(xx=0; xx<size; xx++) {
				*q ^= (*p & 1) << bit;
				bit--;
				if(bit < 0) {
					q++;
					bit = 7;
				}
			}
			p++;
		}
		for(yy=0; yy<size; yy++) {
			png_write_row(png_ptr, row);
		}
	}
	/* bottom margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	free(row);
	free(palette);
}
