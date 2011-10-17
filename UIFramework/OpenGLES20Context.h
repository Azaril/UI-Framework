#pragma once

class COpenGLES20Context
{
    public:
        virtual int AddRef(
            ) = 0;
    
        virtual int Release(
            ) = 0;
    
        virtual bool Apply(
            ) = 0;	
};