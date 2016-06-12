

#ifndef TIFFIMAGEOFILEFACTORY_CPP_
#define TIFFIMAGEOFILEFACTORY_CPP_


#include "TIFFImageOFileFactory.h"


using namespace ImageOFactory;

//******************************************************************
TIFFImageOFileFactory::TIFFImageOFileFactory() : ImageOFileFactory()
{}

//******************************************************************
TIFFImageOFileFactory::~TIFFImageOFileFactory()
{}


//*******************************************************************
USGSImageLib::ImageOFile * TIFFImageOFileFactory::
create(std::string & filename,
       const int & width, const int & height, const int & photo)
  throw (ImageOFactoryException)
{
  
  USGSImageLib::ImageOFile * temp = NULL;             //temporary pointer
  USGSImageLib::TIFFImageOFile * tifout = NULL;       //temp tiff pointer
  
  try
  {
                                                      //now create the file
    if (!(temp = new (std::nothrow) USGSImageLib::TIFFImageOFile
          (filename, width, height, photo)))
      throw std::bad_alloc();
  
  
    if (!temp->good())                                //check file integraty
      return NULL;                                    //could not open the 
                                                      //file

                                                      //set some default flags
    tifout = dynamic_cast<USGSImageLib::TIFFImageOFile*>(temp);
    tifout->setPlanarConfig(1);
    tifout->setSubfileType(0);
    tifout->setCompression(COMPRESSION_NONE);
  
    filelist.push_back(temp);                         //put it in the list

    return temp;                                      //done
  }
  catch(std::bad_alloc & badallc)
  {
    throw ImageOFactoryException(TIFFFACTORY_NOMEM); //throw error
  }
  catch(...)
  {    
    throw ImageOFactoryException(TIFFFACTORY_UNKOWN);//throw unkown
  }
}



#endif

