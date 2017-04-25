#include <iostream>
#include "../include/convcudamatrix.h"


void cudaConvMatrix::_init(float *data, int bs, int height, int width, int channels,int pad, int kern_sz, int stride){
	devData = NULL;
	_bs = bs;
   _height = height;
   _width = width;
   _ch_in = channels;
   _pad = pad;
   _kern_sz = kern_sz;
   _stride = stride;
   n_elems = _height*_width*_ch_in;
  cudaError_t err;
  // setbestcudaDevice();
  // cudaSetDevice(gpuid);
  if (data != NULL) {
    std::cout << "got data!\n";
    if (numElems > 0) {
      cudaMalloc((void**)&devData, n_elems*sizeof(float));
      cudaMemcpy(devData,data,n_elems*sizeof(float),cudaMemcpyHostToDevice);
      err = cudaGetLastError();
      if (err != cudaSuccess) {
       std::cout << "Couldn't allocate memory\n";
      }
    }
  }
  else {
    std::cout << "empty data!\n";
    if (numElems > 0) {
      cudaMalloc((void**)&devData, n_elems*sizeof(float));
      err = cudaGetLastError();
      if (err != cudaSuccess) {
       std::cout << "Couldn't allocate memory\n";
      }
    }
  }
}


cudaConvMatrix::cudaConvMatrix(){
	_init(NULL,0,0,0,0,0,0,0);
}

cudaConvMatrix::cudaConvMatrix(float *data, int bs, int height, int width, int channels){
	_init(data,bs,height,width,channels,1,3,1);
}

cudaConvMatrix::cudaConvMatrix(int bs, int height, int width, int channels,int pad, int kern_sz,int stride){
	_init(NULL,bs,height,width, channels,pad, kern_sz,stride);
}

// void cudaConvMatrix::gemm_conv_gpu(bool tA, bool tB, const cudaConvMatrix &b, float scaleA, float scaleB, cudaConvMatrix &tgt int offset){
// cudaError_t err;
//   int m = b.getnDim(2);
//   int n = _height;
//   int k = _width;
//   // float *rsa = reshape_data(devData,numRows,numCols);
//   // float *rsb = reshape_data(b.getDevData(),b.getNumRows(), b.getNumCols());
//   cublasHandle_t handle;
//   cublasCreate(&handle);
//   // if ((tgt.getNumCols() != b.getNumCols()) && (tgt.getNumRows()!=numRows)) {
//   //    std::cout << "Matrix dimensions are not same .. aborting \n";
//   //    exit(1);
//   // }

//     cublasSgemm(handle,(tA?CUBLAS_OP_T:CUBLAS_OP_N),(tB?CUBLAS_OP_T:CUBLAS_OP_N),
//                 m,n,k,&scaleA,b.getDevData(),b.getnDim(2),devData,_width,&scaleB,tgt.getDevData()+offset,b.getNumCols());
//     err = cudaGetLastError();
//     if (err != cudaSuccess) {
//       printf("Cannot do Sgemm..\n");
//     }
// }


void cudaConvMatrix::fwd_pass_convolution_gpu(cudaConvMatrix &weights, cudaConvMatrix &tgt){
	int col_height = ((_height-_kern_sz/2)/_stride)*((_width-_kern_sz/2)/_stride);
	int col_width = _kern_sz*_kern_sz*_ch_in;
	float *col;
    cublasHandle_t handle;
    cublasCreate(&handle);
    int m = getnDim(2);
    int n = col_height*col_width;
    int k = _kern_sz*_kern_sz*_ch_in;
    float scaleA = 1.f;
    float scaleB = 1.f;
	cudaMalloc((void**)&col,col_width*col_height*sizeof(float));
	for (int i = 0; i < _bs; ++i)
	{
	   im2col_gpu(devData+i*_height*_width*_ch_in,_ch_in, _height,_width,_kern_sz,_stride,_pad,col_height,col_width,col);
       cublasSgemm(handle,CUBLAS_OP_N, CUBLAS_OP_N,m,n,k,&scaleA,weights.getDevData(),k);  	   
    }


}