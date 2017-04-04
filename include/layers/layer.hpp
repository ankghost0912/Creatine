#ifndef LAYER_H
#define LAYER_H

#include "../matrix.hpp"
#include "../utils.hpp"
#include <string>
#include <map>
#include <assert.h>
#include <iostream>
typedef long long int int64;


class ip_layer {
protected:
  Matrix* _input;
  Matrix* _output;   // a pointer so that could be used for later layers
  Matrix* _parGrads; // partial gradients
  void _init(int64 input_s);
public:
  ip_layer();
  ip_layer(int64 input_s);
  void peek();
  void feed(double* input);
  void forward(PASS_TYPE pass_type);
  void backward(PASS_TYPE pass_type);
  Matrix* getFprop() const;
  Matrix* getBprop() const;
  ~ip_layer();
};

class fc_layer {
protected:
  Matrix* _input;
  Matrix* _output;   // a pointer so that could be used for later layers
  double* weight_init;
  double* bias_init;
  Matrix* _weight;
  Matrix* _bias;
  Matrix* _parGrads_w; // partial gradients
  Matrix* _parGrads_b;

  void _init(int64 input_s, int64 output_s, bool biasThis);
public:
  fc_layer();
  fc_layer(int64 input_s, int64 output_s);
  fc_layer(int64 input_s, int64 output_s, bool biasThis);
  void feed(Matrix* input);
  void forward(PASS_TYPE pass_type);
  void backward(PASS_TYPE pass_type);
  Matrix* getFprop();
  Matrix* getBprop();
  ~fc_layer();
};

class softmax_layer {
protected:
  Matrix* _input;
  Matrix* _output;   // a pointer so that could be used for later layers
  Matrix* _parGrads; // partial gradients
  Matrix * _temp_exp;
  double * _temp_exp_sum;
  void _init(int64 input_s);
public:
  softmax_layer();
  softmax_layer(int64 input_s);
  void feed(Matrix* input);
  void forward(PASS_TYPE pass_type);
  void backward(PASS_TYPE pass_type);
  Matrix* getFprop();
  Matrix* getBprop();
  ~softmax_layer();
};

class cross_entropy {
protected:
  Matrix* _input;
  Matrix* _output;   // a pointer so that could be used for later layers
  Matrix* _parGrads; // partial gradients
  Matrix * _temp_exp;
  double * _temp_exp_sum;
  void _init(int64 input_s);
public:
  cross_entropy();
  cross_entropy(int64 input_s);
  void feed(Matrix* input);
  void forward(PASS_TYPE pass_type);
  void backward(PASS_TYPE pass_type);
  Matrix* getFprop();
  Matrix* getBprop();
  ~cross_entropy();
}


#endif

