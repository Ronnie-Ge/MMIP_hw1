#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//宣告函數
int is_raw_file(const char *filename);
int is_bmp_file(const char *filename);
void read_raw(const char *filename, unsigned char *data, int width, int height);
void read_bmp(const char *filename, unsigned char *data, int width, int height, unsigned char *header);
void print_matrix(unsigned char *data,int width,int height);
void print_matrix_center(unsigned char *data, int width, int height);
void apply_transforms(unsigned char *data, unsigned char *log_img,
                      unsigned char *gamma_img, unsigned char *neg_img
                      ,int width,int height);
void save_raw(unsigned char *data, int width, int height, char *filename);
void save_bmp(unsigned char *data, int width, int height, char *filename, unsigned char *header);
void NN_sampling(unsigned char *data, unsigned char *sampling_data, int old_width, int old_height, int new_width, int new_height);
void bilinear_sampling(unsigned char *data, unsigned char *sampling_data, int old_width, int old_height, int new_width, int new_height);
#endif
