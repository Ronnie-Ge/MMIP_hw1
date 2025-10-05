#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
//原圖寬度高度
#define WIDTH 512            //problem_c需要在這裡更改寬度
#define HEIGHT 512           //problem_c需要在這裡更改高度

int main() {
    //problem_c需要在這裡更改檔案夾名稱
    char *filenames[] = {"./32_32/baboon.bmp", "./32_32/boat.bmp", "./32_32/F16.bmp",      
                        "./data/goldhill.raw", "./data/lena.raw", "./data/peppers.raw"};
                        
    for (int i = 0; i < 6; i++) {
        int scale = 16;      //problem_c需要在這裡更改scale，調整圖片大小
        unsigned char *data = (unsigned char*)malloc(WIDTH*HEIGHT);
        char output_file[256];          //儲存圖片檔名
        unsigned char header[1078];      //儲存BMP檔頭

        if (is_raw_file(filenames[i])) read_raw(filenames[i], data, WIDTH, HEIGHT);
        else if (is_bmp_file(filenames[i])) read_bmp(filenames[i], data, WIDTH, HEIGHT,header);
        
        printf("Centered 10x10 pixels:\n");
        print_matrix_center(data, WIDTH, HEIGHT);


        // 分配轉換圖像空間
        unsigned char *log_img   = malloc(WIDTH*HEIGHT);
        unsigned char *gamma_img = malloc(WIDTH*HEIGHT);
        unsigned char *neg_img   = malloc(WIDTH*HEIGHT);
    
        apply_transforms(data, log_img, gamma_img, neg_img, WIDTH, HEIGHT);
        
        //儲存圖片  
        char temp[256];
        strcpy(temp, filenames[i]);             // 複製一份檔名
        char *basename = strtok(temp, ".");     // 拿掉副檔名
        printf("basename: %s\n", basename);
        if (is_raw_file(filenames[i])) {
            unsigned char *NN_sampling_img = (unsigned char*)malloc(WIDTH/scale*HEIGHT/scale);
            unsigned char *bilinear_sampling_img = (unsigned char*)malloc(WIDTH/scale*HEIGHT/scale);
            //NN_sampling(data, NN_sampling_img, WIDTH, HEIGHT, 2*WIDTH/scale, HEIGHT/scale);
            //bilinear_sampling(data, bilinear_sampling_img, WIDTH, HEIGHT, 2*WIDTH/scale, HEIGHT/scale);
            //snprintf(output_file, 256, ".%s_NN_sampling_img.raw",  basename);
            //save_raw(NN_sampling_img, WIDTH/scale, HEIGHT/scale, output_file);
            //snprintf(output_file, 256, ".%s_bilinear_sampling_img.raw",  basename);
            //save_raw(bilinear_sampling_img, WIDTH/scale, HEIGHT/scale,output_file);
            /*snprintf(output_file, 256, ".%s_log_output.bmp",  basename);
            save_raw(log_img, WIDTH, HEIGHT, output_file);
            snprintf(output_file, 256, ".%s_gamma_output.bmp",  basename);
            save_raw(gamma_img, WIDTH, HEIGHT, output_file);
            snprintf(output_file, 256, ".%s_neg_output.bmp",  basename);
            save_raw(neg_img, WIDTH, HEIGHT, output_file);*/
            free(NN_sampling_img);
            free(bilinear_sampling_img);    
        }   
        else if (is_bmp_file(filenames[i])) {
            unsigned char *NN_sampling_img = (unsigned char*)malloc(WIDTH/scale*HEIGHT/scale);
            unsigned char *bilinear_sampling_img = (unsigned char*)malloc(WIDTH/scale*HEIGHT/scale);
            NN_sampling(data, NN_sampling_img, WIDTH, HEIGHT, WIDTH/scale, HEIGHT/scale);
            bilinear_sampling(data, bilinear_sampling_img, WIDTH, HEIGHT, WIDTH/scale, HEIGHT/scale);
            snprintf(output_file, 256, ".%s_NN_sampling_img_.bmp",  basename);
            save_bmp(NN_sampling_img, WIDTH/scale, HEIGHT/scale, output_file, header);
            snprintf(output_file, 256, ".%s_bilinear_sampling_img.bmp",  basename);
            save_bmp(bilinear_sampling_img, WIDTH/scale, HEIGHT/scale, output_file, header);
            //snprintf(output_file, 256, ".%s_log_output.bmp",  basename);
            //save_bmp(log_img, WIDTH, HEIGHT, output_file, header);
            //snprintf(output_file, 256, ".%s_gamma_output.bmp",  basename);
            //save_bmp(gamma_img, WIDTH, HEIGHT, output_file, header);
            //snprintf(output_file, 256, ".%s_neg_output.bmp",  basename);
            //printf("neg_Centered 10x10 pixels:\n");
            //print_matrix_center(neg_img, WIDTH, HEIGHT);
            //save_bmp(neg_img, WIDTH, HEIGHT, output_file, header);
            free(NN_sampling_img);
            free(bilinear_sampling_img);
        }
        
        free(log_img);
        free(gamma_img);
        free(neg_img);
        free(data);
    }
}