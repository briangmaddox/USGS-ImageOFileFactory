#ifndef IMAGEOFACTORYEXCEPTION_CPP_
#define IMAGEOFACTORYEXCEPTION_CPP_


#include "ImageOFactoryException.h"


using namespace ImageOFactory;

//***************************************************
ImageOFactoryException::ImageOFactoryException()
{
  error = IMAGEOFACTORY_DEFAULT;
}

//***************************************************
ImageOFactoryException::ImageOFactoryException(std::string inerror)
{
  error = inerror;
}
 
//**********************************************************
ImageOFactoryException::~ImageOFactoryException()
{
}

//**********************************************************
std::string ImageOFactoryException::get_message() const throw()
{
  return error;
}
  
//************************************************************
void ImageOFactoryException::set_message(std::string inerror) throw()
{
  error = inerror;
}

#endif
