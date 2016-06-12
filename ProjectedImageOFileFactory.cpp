#ifndef PROJECTEDIMAGEOFILEFACTORY_CPP_
#define PROJECTEDIMAGEOFILEFACTORY_CPP_


#include "ProjectedImageOFileFactory.h"


using namespace ImageOFactory;

//*****************************************************************
ProjectedImageOFileFactory::ProjectedImageOFileFactory() 
  throw (ImageOFactoryException) :
  ImageOFileFactory()
{
  proj = NULL;                                    //set the proj to NULL
  scale.x = scale.y = 1.0;                        //set the scale
                                                  //create the default size
  if (!(tiepoints = new (std::nothrow) double [6]))
    throw ImageOFactoryException(PROJIMAGE_NOMEM);
  
  tiepointnum = 6;                                //set the current number
  
  
}


//***************************************************************** 
ProjectedImageOFileFactory::~ProjectedImageOFileFactory()
{
  if (tiepoints)                                  //delete the tiepoints
    delete [] tiepoints;
}

//*****************************************************************
void ProjectedImageOFileFactory::
set_projection_info(ProjLib::Projection * inproj,
                         const MathLib::Point & inscale,
                         const double * intiepoints,
                         const int & intiepointnum)
  throw(ImageOFactoryException)
{
  int counter;                                    //counter for loop

  if (!inproj)                                    //check for a valid proj
    throw ImageOFactoryException(PROJIMAGE_INVALIDPROJ);
  
  proj = inproj;                                  //weak copy
  scale = inscale;                                //get scale
  
  if ( intiepointnum != tiepointnum)               //check tiepoints
  {
    if (intiepointnum <= 6)                        //can't be less than 6
      throw ImageOFactoryException(PROJIMAGE_LESSTHAN6);
    
    delete [] tiepoints;                          //delete the old tiepoints
    tiepointnum = intiepointnum;

    if (!(tiepoints = new (std::nothrow) double [tiepointnum]))
      throw ImageOFactoryException(PROJIMAGE_NOMEM);
    
  }

  for (counter = 0; counter < tiepointnum; counter++)
    tiepoints[counter] = intiepoints[counter];

}

#endif

