#ifndef PROJECTEDIMAGEOFILEFACTORY_H_
#define PROJECTEDIMAGEOFILEFACTORY_H_



#include "ImageOFileFactory.h"
#include "ProjectionLib/Projection.h"
#include "MathLib/Point.h"


/** Define some errors for use with the class */
#define PROJIMAGE_NOMEM "ProjectedImageOFileFactory: no memory"
#define PROJIMAGE_INVALIDPROJ "ProjectedImageOFileFactory: Invalid Projection"
#define PROJIMAGE_LESSTHAN6 "ProjectedImageOFileFactory: Less than 6 ties"




namespace ImageOFactory
{


class ProjectedImageOFileFactory : public  ImageOFileFactory
{

public:
  /** Main Constructor for the class */
  ProjectedImageOFileFactory() throw (ImageOFactoryException);
  /** Main Destructor for the class */
  ~ProjectedImageOFileFactory();

  /** set_projection_info changes the projection info
      that is used when creating a projected file. */
  void set_projection_info(ProjLib::Projection * inproj,
                           const MathLib::Point & inscale,
                           const double * intiepoints,
                           const int & intiepointnum)
    throw(ImageOFactoryException);

  /** create function derived from base class will
      throw if the projection info is not set **/
  virtual USGSImageLib::ImageOFile * create(std::string & filename,
              const int & width, const int & height, const int & photo)
    throw (ImageOFactoryException) = 0;

  /** overloaded create function allows you to specify the 
      projection info during creation time.  This will also
      "set" the internal projection info so if a new file is
      created it will have the same projection info unless it
      is changed. */
  virtual USGSImageLib::ImageOFile * create(std::string & filename,
      const int & width, const int & height, const int & photo,
      ProjLib::Projection * inproj, const MathLib::Point & inscale,
      const double * intiepoints, const int & intiepointnum)
    throw(ImageOFactoryException) =0;
  
protected:
  
  /** projection type to create */
  ProjLib::Projection * proj;
  /** output scale */
  MathLib::Point scale;
  /** tiepoints   */
  double * tiepoints;
  /** tiepoint number */
  int tiepointnum;
};

}//namespace

#endif
