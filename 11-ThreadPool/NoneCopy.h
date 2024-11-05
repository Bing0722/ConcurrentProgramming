#ifndef __NoneCopy_H_
#define __NoneCopy_H_

class NoneCopy{
public:
    ~NoneCopy(){}
protected:
    NoneCopy(){}
private:
    NoneCopy(const NoneCopy&) = delete;
    NoneCopy& operator=(const NoneCopy&) = delete;
};

#endif