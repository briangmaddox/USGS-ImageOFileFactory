#ifndef TIFFIMAGEOFILEFACTORY_H_
#define TIFFIMAGEOFILEFACTORY_H_

#include "ImageOFileFactory.h"
#include "TIFFImageOFile.h"

/** Define some error for use with the class */
#define TIFFFACTORY_NOMEM  "TIFFImageOFileFactory: No memory"
#define TIFFFACTORY_UNKOWN "TIFFImageOFileFactory: Unkown error"


namespace ImageOFactory
{

/**Base Class that creates and manages tiffImageOFiles
  *@author Chris Bilderback
  */
class TIFFImageOFileFactory : public ImageOFileFactory
{  

public:
  /** Main constructor and destructor for the class */
  TIFFImageOFileFactory();
 
  virtual ~TIFFImageOFileFactory();


  /** This function creates a TIFFImageOFile with default parameters
      and returns it.  If the function is unable to create a 
      file it returns a NULL pointer.*/
  virtual USGSImageLib::ImageOFile * create(std::string & filename,
                const int & width, const int & height, const int & photo)
    throw (ImageOFactoryException);


};


}//Namespace

#endif
