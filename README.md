# MMIP_hw1
程式架構：
.
├── main.c              # 主程式：負責呼叫各種影像處理流程
├── utils.c             # 函式庫實作檔案
├── utils.h             # 函式宣告與定義
├── data/               # 放置原圖(三張Raw,三張bmp)
└── problem_(a,b,c)     # 執行檔

參數調整：
針對每個題目要求可以到main.c裡面調整以下參數：
1. WIDTH / HEIGHT	原圖寬高（需與輸入圖一致）	
2. scale	縮放比例（越大 → 縮得越小）	
3. filenames[]: 輸入和輸出的檔案路徑和對應資料夾
4. 需要手動調整寬高不同的情況，例如NN_sampling(data, NN_sampling_img, WIDTH, HEIGHT, WIDTH/scale, HEIGHT/scale)
   要把WIDTH/scale改成2*WIDTH/scale，這是當初在寫函數時的一點小失誤，應該調成output_width會更好。

程式使用：
編譯： gcc main.c utils.c -o output -lm
如果要使用執行檔就直接執行./problem_(a,b,c)即可
