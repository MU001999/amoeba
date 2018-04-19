#ifndef __ICE_OBJECT_H__
#define __ICE_OBJECT_H__

#include <iostream>
#include <string>
#include <cstring>

class IceObject
{
public:
    enum class TYPE
    {
        INT,
        DOUBLE,
        STRING
    } type;
    virtual ~IceObject() {}
    virtual void show() = 0;
    virtual IceObject *add(IceObject *) = 0;
    virtual IceObject *mul(IceObject *) = 0;
};

class IceIntegerObject : public IceObject
{
private:
    long value;

public:
    IceIntegerObject(long value);
    virtual ~IceIntegerObject() {}
    virtual void show();
    virtual IceObject *add(IceObject *);
    virtual IceObject *mul(IceObject *);
};

#endif // __ICE_OBJECT_H__