#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


__global__ void imageblur( int* inputImage, int* outputImage, int filterSize,  double* filter, int imageRow, int imageCol){
  
  int pixelx = blockIdx.x * blockDim.x + threadIdx.x;
  int pixely = blockIdx.y * blockDim.y + threadIdx.y;
  double blur_value = 0.0;  
  
  if (pixelx >= imageCol || pixely >= imageRow) {
      return;
  }

  //multiply with blur kernel
  for (int finalx = 0; finalx < filterSize; finalx++) {
    for (int finaly = 0; finaly < filterSize; finaly++) {
      int imagex = pixelx + finalx - filterSize / 2 ;
      int imagey = pixely + finaly - filterSize / 2;
      int imagePixel;
      if(imagex < 0 || imagex >= imageCol || imagey < 0 || imagey >= imageRow){
        imagePixel = 0;
      } else {
        imagePixel = inputImage[imagey*imageCol+imagex];
      }
      blur_value += (filter[finaly*filterSize+finalx] * imagePixel);
    }
  }

  outputImage[pixely*imageCol+pixelx] = (int)(blur_value/15.0);
}

int main(int argc, char const *argv[]) {

  int imagex = 3, imagey = 3;
  
  int numberOfPixels = imagex*imagey*sizeof(int);
  
  int *d_image = 0; int *d_blurImage = 0; double *d_filter = 0; //device
  int *h_image = 0; int *h_blurImage = 0; double *h_filter = 0; //host
  
  //malloc memory device and host
  h_image = (int*)malloc(numberOfPixels);
  cudaMalloc((void**)&d_image, numberOfPixels);
  h_blurImage = (int*)malloc(numberOfPixels);
  cudaMalloc((void**)&d_blurImage, numberOfPixels);
  h_filter = (double*)malloc(9*sizeof(double));
  cudaMalloc((void**)&d_filter, 9*sizeof(double));

  if(h_image == 0 || d_image == 0 || h_blurImage == 0 || d_blurImage == 0){
    printf("Could not allocate memory");
    return 1;
  }
  
  //Initialise Filter
  h_filter[0] = 1.0; h_filter[1] = 2.0; h_filter[2] = 1.0;
  h_filter[3] = 2.0; h_filter[4] = 3.0; h_filter[5] = 2.0;
  h_filter[6] = 1.0; h_filter[7] = 2.0; h_filter[8] = 1.0;

  // Randomly Initialize Image
  srand(time(NULL));
  for(int i = 0; i < (imagex*imagey); i++){
    h_image[i] = (rand() % 256);
  }
  
  //Copy host memory to device
  cudaMemcpy( d_image, h_image, numberOfPixels, cudaMemcpyHostToDevice);
  cudaMemcpy( d_filter, h_filter, 9*sizeof(double), cudaMemcpyHostToDevice);
  
  const dim3 blockSize(4,4,1);
  const dim3 gridSize(imagex/blockSize.x+1,imagey/blockSize.y+1,1);
  
  //Call
  imageblur<<<gridSize, blockSize>>>(d_image, d_blurImage, 3, d_filter, imagey, imagex);
  
  //copy blurred image to host
  cudaMemcpy(h_blurImage, d_blurImage, numberOfPixels, cudaMemcpyDeviceToHost);

  printf("Image : \n");
  for(int i = 0; i < imagex; i++){
    for(int j = 0; j < imagey; j++){
      printf("%d ", h_image[i*imagex + j]);
    }
    printf("\n");
  }
  printf("Blur Image: \n");

  for(int i = 0; i < imagex; i++){
    for(int j = 0; j < imagey; j++){
      printf("%d ", h_blurImage[i*imagex + j]);
    }
    printf("\n");
  }
  
  //Clean Memory
  free(h_image); free(h_blurImage); free(h_filter);
  cudaFree(d_image); cudaFree(d_blurImage); cudaFree(d_filter);

  return 0;
}

