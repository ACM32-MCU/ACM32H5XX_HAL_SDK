/******************************************************************************
*@file  : cordic_app.h
*@brief : header file
******************************************************************************/
#ifndef __CORDIC_APP_H__
#define __CORDIC_APP_H__

#include "hal.h"

/******************************************************************************
*@brief : function for cosin/sin calculation in float format 
*  
*@param : angle_f       : input angle data in radians, divided by ¦Ð, range(-1.0,1.0)
*@param : cos_f         : the cos value of the input angle in float format, range(-1.0,1.0)
*@param : sin_f         : the sin value of the input angle in float format, range(-1.0,1.0)
*@return: None
******************************************************************************/
void CORDIC_CosSin_Float(float angle_f, float* cos_f, float* sin_f);

/******************************************************************************
*@brief : calculate the atan & sqrt value of the input x,y in float format
*         
*@param : x_f           : input x in float format, range(-1,1)
*@param : y_f           : input y in float format, range(-1,1)
*@param : sqrt_f        : the sqrt value of the input x,y in float format, range[0,1]
*                         sqrt_data should devide (CORDIC_F_31>>4) to obtain the value in float
*                         if the modulus of input vector is greator than 1, the sqrt result will be saturated to 1
*@param : atan_f        : the atan value of the input y/x in float in float format, range[0,1]
*                         atan_value must be multiplied by ¦Ð to obtain the angle in radians
*@return: None
******************************************************************************/
void CORDIC_AtanSqrt_Float(float x_f, float y_f, float* sqrt_f, float* atan_f);

/******************************************************************************
*@brief : calculate the sinh & cosh value of the input angle in float format
*         
*@param : x_f          :  input hyperbolic angle data in radians, range [-0.559 ~0. 559]([-1.118 ~ 1.118]/2), float format 
*@param : cosh_f       :  the hyperbolic cosh value of the input angle,range[0.5 0.846], float format
*                         cosh_data must be multiplied by 2 to obtain the correct result
*@param : sinh_f       :  the hyperbolic sinh value of the input angle,range[-0.683 0.683], float format
*                         sinh_data must be multiplied by 2 to obtain the correct result
*@return: None
******************************************************************************/
void CORDIC_CoshSinh_Float(float x_f, float* cosh_f, float* sinh_f);

/******************************************************************************
*@brief : calculate the hyperbolic arctangent of the input argument in float format 
*         
*@param : x_f          :  input hyperbolic angle data,range [-0.403 0.403]([-0.806 ~ 0.806]/2),float format;
*@param : atanh_f      :  the atanh value of the input argument,float format
                          atanh_value must be multiplied by 2 to obtain the correct result
*@return: None
******************************************************************************/
void CORDIC_Atanh_Float(float x_f, float* atanh_f);

/******************************************************************************
*@brief : calculate the natural logarithm of the input argument in float format 
*         
*@param : x_f          :  input argument,range [0.054 0.875],float format;
*@param : ln_f         :  the natural logarithm value of the input argument,float format
*                         ln_value must be multiplied by 4 to obtain the correct result.
*@return: None
******************************************************************************/
//ORIGIN RANGE         SCALE       ARG RANGE
//0.107 ¡Ü x < 1          1         0.0535 ¡Ü ARG1 < 0.5
//1 ¡Ü x < 3              2         0.25 ¡Ü ARG1 < 0.75
//3 ¡Ü x < 7              3         0.375 ¡Ü ARG1 < 0.875
//7 ¡Ü x ¡Ü 9.35           4         0.4375 ¡Ü ARG1 < 0.584
void CORDIC_Ln_Float(float x_f, float* ln_f);

/******************************************************************************
*@brief : calculate the square root value of the input argument in float format 
*         
*@param : x_f          :  input argument,range [0.027 0.875],float format;
*@param : sqrt_f       :  the squart root value of the input argument,float format
*                         sqrt_value must be multiplied by 2^scale/W_INV_Q31 to obtain the correct result
*@return: None
******************************************************************************/
//ORIGIN RANGE         SCALE             ARG RANGE
//0.027 ¡Ü x < 0.75       0         0.027 ¡Ü ARG1 < 0.75
//0.75  ¡Ü x < 1.75       1         0.375 ¡Ü ARG1 < 0.875
//1.75  ¡Ü x ¡Ü 2.341     2         0.4375¡Ü ARG1 ¡Ü 0.585
void CORDIC_Sqrt_Float(float x_f, float* sqrt_f);
#endif
