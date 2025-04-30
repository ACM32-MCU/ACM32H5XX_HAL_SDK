/******************************************************************************
*@file  : app.c
*@brief : Cordic Demos 
******************************************************************************/
#include "app.h"
#include "math.h"
#include "cordic_app.h"

void CORDIC_CosSin_Test(void)
{
    float test_angle[9] = {-1, -0.7, -0.5, -0.2, 0 , 0.3, 0.5, 0.8, 1};//angle data is in radius and should be devided by PI
    float sinf_cordic, cosf_cordic;
    float sinf_math, cosf_math;
    uint8_t err_flag;
    uint8_t i;
       
    err_flag = 0;
    for(i=0; i<9; i++)
    {
        CORDIC_CosSin_Float(test_angle[i], &cosf_cordic, &sinf_cordic);
        
        sinf_math = sin(test_angle[i]*PI);        
        cosf_math = cos(test_angle[i]*PI);
       
        if(fabs(sinf_cordic - sinf_math) > 0.000001) 
        { 
            err_flag = 1;
            printf("sinf_cordic = %f, sin_math = %f\n", sinf_cordic, sinf_math);           
        }
        if(fabs(cosf_cordic - cosf_math) > 0.000001)        
        { 
            err_flag = 1;
            printf("cosf_cordic = %f, cos_math = %f\n", cosf_cordic, cosf_math);           
        }
    }
    
    if(err_flag) printf("CosSin test fail\n");
    else printf("CosSin test pass\n");
}

void CORDIC_AtanSqrt_Test(void)
{
    float test_x[5] = {-1, -0.23, 0, 0.5, 1};  //x data,[-1,1]
    float test_y[5] = {1, -0.8, 0, 0.2, -0.5};//y data,[-1,1]
    float sqrtf_cordic, atanf_cordic;
    float sqrtf_math, atanf_math;
    uint8_t err_flag;

    uint8_t i;
    
    err_flag = 0;
    for(i=0; i<5; i++)
    {
        CORDIC_AtanSqrt_Float(test_x[i], test_y[i], &sqrtf_cordic, &atanf_cordic);
        
        sqrtf_math = hypot(test_x[i],test_y[i]);
        atanf_math = atan2(test_y[i], test_x[i])/PI;
        if(fabs(sqrtf_cordic - sqrtf_math) > 0.000001) 
        { 
            err_flag = 1;
            printf("sqrtf_cordic = %f, sqrtf_math = %f\n", sqrtf_cordic, sqrtf_math);           
        }
        if(fabs(atanf_cordic - atanf_math) > 0.000001)        
        { 
            err_flag = 1;
            printf("atanf_cordic = %f, atanf_math = %f\n", atanf_cordic, atanf_math);           
        }
    }
    
    if(err_flag) printf("AtanSqrt test fail\n");
    else printf("AtanSqrt test pass\n");
}

void CORDIC_CoshSinh_Test(void)
{
    float test_x[5] = {-1.118, -0.56, 0 , 0.86, 1.118};
    float sinhf_cordic,coshf_cordic;
    float sinhf_math,coshf_math;

    uint8_t err_flag;
    uint8_t i;
    
    err_flag = 0;    
    for(i=0; i<5; i++)
    {    
        CORDIC_CoshSinh_Float(test_x[i], &coshf_cordic, &sinhf_cordic);
        sinhf_math = sinh(test_x[i]);        
        coshf_math = cosh(test_x[i]); 
        
        if(fabs(sinhf_cordic - sinhf_math) > 0.000001) 
        { 
            err_flag = 1;
            printf("sinhf_cordic = %f, sinhf_math = %f\n", sinhf_cordic, sinhf_math);           
        }
        if(fabs(coshf_cordic - coshf_math) > 0.000001)        
        { 
            err_flag = 1;
            printf("coshf_cordic = %f, coshf_math = %f\n", coshf_cordic, coshf_math);           
        }
    }
    
    if(err_flag) printf("CoshSinh test fail\n");
    else printf("CoshSinh test pass\n");
}

void CORDIC_Atanh_Test(void)
{
    float test_x[5] = {-0.806, -0.45, 0 , 0.75, 0.806};
    float atanh_cordic;
    float atanh_math;

    uint8_t err_flag;
    uint8_t i;
    
    err_flag = 0; 
    for(i=0; i<5; i++)
    {    
        CORDIC_Atanh_Float(test_x[i], &atanh_cordic);
        
        atanh_math = atanh(test_x[i]);
        if(fabs(atanh_cordic - atanh_math) > 0.000001) 
        { 
            err_flag = 1;
            printf("atanh_cordic = %f, atanh_math = %f\n", atanh_cordic, atanh_math);           
        }
    }
    
    if(err_flag) printf("Atanh test fail\n");
    else printf("Atanh test pass\n");   
}

void CORDIC_Ln_Test(void)
{
    float test_x[9] = {0.107, 0.5, 1, 2, 3, 5, 7, 8, 9.35};
    float lnf_cordic;
    float lnf_math;
    uint8_t err_flag;
    uint8_t i;
   
    err_flag = 0;
    for(i=0; i<9; i++)
    {    
        CORDIC_Ln_Float(test_x[i], &lnf_cordic);
        
        lnf_math = logf(test_x[i]);
        if(fabs(lnf_cordic - lnf_math) > 0.000001) 
        { 
            err_flag = 1;
            printf("lnf_cordic = %f, lnf_math = %f\n", lnf_cordic, lnf_math);           
        }
    }
    
    if(err_flag) printf("Ln test fail\n");
    else printf("Ln test pass\n");          
}

void CORDIC_Sqrt_Test(void)
{
    float test_x[7] = {0.027, 0.5, 0.75 , 1, 1.75, 2, 2.341};
    float sqrtf_cordic;
    float sqrtf_math;
    uint8_t err_flag;
    uint8_t i;
   
    err_flag = 0;    
    for(i=0; i<7; i++)
    {    
        CORDIC_Sqrt_Float(test_x[i], &sqrtf_cordic);        
        sqrtf_math = sqrt(test_x[i]);
        
        if(fabs(sqrtf_cordic - sqrtf_math) > 0.000001) 
        { 
            err_flag = 1;
            printf("sqrtf_cordic = %f, sqrtf_math = %f\n", sqrtf_cordic, sqrtf_math);           
        }
    }
    
    if(err_flag) printf("Sqrt test fail\n");
    else printf("Sqrt test pass\n");            
}
    
void APP_CORDIC_Test(void)
{
    CORDIC_CosSin_Test();
    CORDIC_AtanSqrt_Test();	
    CORDIC_CoshSinh_Test();    
    CORDIC_Atanh_Test();
    CORDIC_Ln_Test();
    CORDIC_Sqrt_Test();   
    printfS("\nCordic test end.\r\n");        
}





