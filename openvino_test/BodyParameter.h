//
//  @ Project : HumanPose3D
//  @ File Name : BodyParameter.h
//  @ Date : 2019/6/11
//  @ Author : lwk
//

#if !defined(_BODYPARAMETER_H)
#define _BODYPARAMETER_H


class BodyParameter {
public:
    void setLength(int,int,float, float);
    float getCenterLength(int,int);
    float getMinLength(int,int);
    float getMaxLength(int,int);
	
private:
    float minLength[25][25];
    float maxLength[25][25];
};

#endif  //_BODYPARAMETER_H
