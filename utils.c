#include "utils.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

int is_raw_file(const char *filename) {
    return strstr(filename, ".raw") != NULL;
}

int is_bmp_file(const char *filename) {
    return strstr(filename, ".bmp") != NULL || strstr(filename, ".BMP") != NULL;
}


void read_raw(const char *filename, unsigned char *data, int width, int height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) { printf("Cannot open %s\n", filename); return; }
    fread(data, 1, width*height, fp);
    fclose(fp);
}

void read_bmp(const char *filename, unsigned char *data, int width, int height, unsigned char *header) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) { printf("Cannot open %s\n", filename); return; }

    unsigned char bmp_header[1078];
    fread(bmp_header, 1, 1078, fp); // 跳過 BMP header
    memcpy(header, bmp_header, 1078);
    for (int y = 0; y < height; y++) {
        fread(data + y*width, 1, width, fp);
    }

    fclose(fp);
}

void print_matrix(unsigned char *data, int width, int height) {
 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            printf("%3d ", data[idx]);
        }
        printf("\n");  // 換行
    }
}

void print_matrix_center(unsigned char *data, int width, int height) {
    int startX = width / 2 - 5;
    int startY = height / 2 - 5;
    for (int mid_y = 0; mid_y < 10; mid_y++) {
        for (int mid_x = 0; mid_x < 10; mid_x++) {
            int idx = (startY + mid_y) * width + (startX + mid_x);
            printf("%3d ", data[idx]);
        }
        printf("\n");
    }
}

void apply_transforms(unsigned char *data, unsigned char *log_img,
                      unsigned char *gamma_img, unsigned char *neg_img
                      ,int width,int height) {
    for (int y = 0; y< height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            log_img[idx] = (unsigned char)(255 * log(data[idx] + 1) / log(256));
            gamma_img[idx] = (unsigned char)(255 * pow(data[idx]/255.0, 0.5));
            neg_img[idx] = 255 - data[idx];
        }
    }
}

void save_raw(unsigned char *data, int width, int height, char *filename) {
    FILE *fp = fopen(filename, "wb"); //建立檔案指標
    if (!fp) {
        printf("Cannot open file %s for writing.\n", filename);
        return;
    }
    fwrite(data,sizeof(unsigned char),width*height,fp);
    fclose(fp);
}

void save_bmp(unsigned char *data, int width, int height, char *filename, unsigned char *header) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Cannot open file %s for writing.\n", filename);
        return;
    }
    unsigned char *new_header = malloc(1078);
    memcpy(new_header, header, 1078);
    int img_size = width*height;
    int file_size = 1078 + img_size;
     // 檔案大小 (offset 2 ~ 5)
     new_header[2] = (unsigned char)(file_size      );
     new_header[3] = (unsigned char)(file_size >>  8);
     new_header[4] = (unsigned char)(file_size >> 16);
     new_header[5] = (unsigned char)(file_size >> 24);
 
     // 寬度 (offset 18 ~ 21)
     new_header[18] = (unsigned char)(width      );
     new_header[19] = (unsigned char)(width >>  8);
     new_header[20] = (unsigned char)(width >> 16);
     new_header[21] = (unsigned char)(width >> 24);
 
     // 高度 (offset 22 ~ 25)
     new_header[22] = (unsigned char)(height      );
     new_header[23] = (unsigned char)(height >>  8);
     new_header[24] = (unsigned char)(height >> 16);
     new_header[25] = (unsigned char)(height >> 24);
 
     // 影像大小 (offset 34 ~ 37)
     new_header[34] = (unsigned char)(img_size      );
     new_header[35] = (unsigned char)(img_size >>  8);
     new_header[36] = (unsigned char)(img_size >> 16);
     new_header[37] = (unsigned char)(img_size >> 24); 
    fwrite(new_header, sizeof(unsigned char), 1078, fp);
    fwrite(data, sizeof(unsigned char), width*height, fp);
    fclose(fp);
}
    

void NN_sampling(unsigned char *data, unsigned char *sampling_data, int old_width, int old_height, int new_width, int new_height) {
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            //去找對應原圖的座標
            int ori_x = x * old_width / new_width;
            int ori_y = y * old_height / new_height;
            int idx = y * new_width + x;
            int ori_idx = ori_y * old_width + ori_x;
            sampling_data[idx] = data[ori_y * old_width + ori_x];
        }
    }
}

void bilinear_sampling(unsigned char *data, unsigned char *sampling_data,
                       int old_width, int old_height,
                       int new_width, int new_height) {
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            //計算原圖座標
            float gx = (float)x * (old_width - 1) / (new_width - 1);
            float gy = (float)y * (old_height - 1) / (new_height - 1);

            int x0 = (int)gx;
            int y0 = (int)gy;
            int x1 = (x0 + 1 < old_width) ? x0 + 1 : x0;    //做x邊界判斷
            int y1 = (y0 + 1 < old_height) ? y0 + 1 : y0;   //做y邊界判斷

            float dx = gx - x0;   
            float dy = gy - y0;   

            unsigned char v00 = data[y0 * old_width + x0];
            unsigned char v01 = data[y0 * old_width + x1];
            unsigned char v10 = data[y1 * old_width + x0];
            unsigned char v11 = data[y1 * old_width + x1];

            float val = (1-dx)*(1-dy)*v00 + dx*(1-dy)*v01 +
                        (1-dx)*dy*v10   + dx*dy*v11;

            sampling_data[y * new_width + x] = (unsigned char)(val + 0.5f);
        }
    }
}
