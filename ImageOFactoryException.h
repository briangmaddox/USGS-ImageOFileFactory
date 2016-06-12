#ifndef IMAGEOFACTORYEXCEPTION_H_
#define IMAGEOFACTORYEXCEPTION_H_

#include <string>


namespace ImageOFactory
{

#define IMAGEOFACTORY_DEFAULT "ImageOFactory: Unkown Error"


/** This class represents a exception that
    can occur while creating a ImageOFile */
class ImageOFactoryException 
{
public:
  /** Main constructor for the class 
      defaults to unkown error */
  ImageOFactoryException();

  /** Secondary constructor for the class 
      takes a error message. */
  ImageOFactoryException(std::string  inerror);
  
  /** Main destructor for the class */
  ~ImageOFactoryException();

  /** get message returns the stored error message */
  std::string get_message() const throw();
  
  /** set message sets the stored error message */
  void set_message(std::string  inerror) throw();

protected:
  
  std::string error;          //the reported error
};

}//Namespace

#endif
  
