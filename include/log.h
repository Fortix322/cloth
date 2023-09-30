#pragma once

#include <Windows.h>

#include "GL/glew.h"

#ifdef _DEBUG

	#include <iostream>

    #define WAIT system("pause");

	#define LOG_ERR(msg, value) std::wcout << "ERROR(" << __FILE__ << ":" << __LINE__ << "): "  << msg << " Value: " << value << std::endl;\
       

	#define CHECK_FOR_ERROR(value, err_value, msg) \
		if(value == err_value)\
		{\
			LOG_ERR(msg, value)\
            WAIT;\
		}

    #define LOG_INFO(msg) std::wcout << "INFO(" << __FILE__ << ":" << __LINE__ << "): " << msg << std::endl; 

    #define GLCall(x) GLClearError();\
        x;\
        if(!GLLogCall()) __debugbreak();
       
#else
	#define LOG_ERR(msg, value)

	#define CHECK_FOR_ERROR(value, err_value, msg) value;

    #define LOG_INFO(msg)

    #define GLCall(x) x;

    #define WAIT

#endif

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall()
{
    while (GLenum error = glGetError())
    {
        LOG_ERR("OpenGL Error.", error);
        return false;
    }
    return true;
}

std::string GetLastErrorAsString()
{
    DWORD errorMessageID = GetLastError();
    if(errorMessageID == 0) {
        return std::string(); 
    }
    
    LPSTR messageBuffer = nullptr;

    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    std::string message(messageBuffer, size);
    if (size)
    {
        message.pop_back(); // Delete \n
        message.pop_back(); // Delete \r
    }

    LocalFree(messageBuffer);
            
    return message;
}