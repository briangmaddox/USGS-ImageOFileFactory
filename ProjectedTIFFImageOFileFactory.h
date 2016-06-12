#ifndef PROJECTEDTIFFIMAGEOFILEFACTORY_H
#define PROJECTEDTIFFIMAGEOFILEFACTORY_H 



#include "ProjectedImageOFileFactory.h"
#include "TIFFImageOFile.h"

/** Define some error messages for use with the class */
#define TIFFPROJ_PROJNOTSET "ProjectedTIFFImageOFileFactory:Projection not set"
#define TIFFPROJ_BADTIE "ProjectedTIFFImageOFileFactory: Bad tie points"
#define TIFFPROJ_UNKOWN "ProjectedTIFFImageOFileFactory: Unkown Error"
#define TIFFPROJ_NOTFW  "ProjectedTIFFImageOFileFactory: Couldn't write TFW"
#define TIFFPROJ_NOMEM  "ProjectedTIFFImageOFileFactory: No memory"
#define TIFFPROJ_BADFILE "ProjectedTIFFImageOFileFactory: Can't open file"



namespace ImageOFactory
{


/** This class writes projected tiff file by just writing a arc world 
    file and creating a normal TIFFImageOFile. */
class ProjectedTIFFImageOFileFactory : public ProjectedImageOFileFactory
{
public:
  
  /** Main constructor for the class */
  ProjectedTIFFImageOFileFactory() throw (ImageOFactoryException);
  
  /** Main destructor for the class */
  virtual ~ProjectedTIFFImageOFileFactory();
  
  /** create function derived from base class will
      throw if the projection info is not set **/
  virtual USGSImageLib::ImageOFile * create(std::string & filename,
              const int & width, const int & height, const int & photo)
    throw (ImageOFactoryException);

  /** overloaded create function allows you to specify the 
      projection info during creation time.  This will also
      "set" the internal projection info so if a new file is
      created it will have the same projection info unless it
      is changed. */
  virtual USGSImageLib::ImageOFile * create(std::string & filename,
      const int & width, const int & height, const int & photo,
      ProjLib::Projection * inproj, const MathLib::Point & inscale,
      const double * intiepoints, const int & intiepointnum)
    throw(ImageOFactoryException);

protected:
  /** writeTWF writes a arc world file based on the tie points. */
  void writeTFW(const std::string & filename) throw (ImageOFactoryException);

};

} //namespace

#endif
