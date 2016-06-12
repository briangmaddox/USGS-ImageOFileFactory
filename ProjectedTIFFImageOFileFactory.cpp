#ifndef PROJECTEDTIFFIMAGEOFILEFACTORY_CPP_
#define PROJECTEDTIFFIMAGEOFILEFACTORY_CPP_

#include "ProjectedTIFFImageOFileFactory.h"
#include <fstream>


using namespace ImageOFactory;

//***********************************************************************
ProjectedTIFFImageOFileFactory::
ProjectedTIFFImageOFileFactory() throw (ImageOFactoryException)
  : ProjectedImageOFileFactory()
{}

//***********************************************************************  
ProjectedTIFFImageOFileFactory::
~ProjectedTIFFImageOFileFactory()
{}

//**********************************************************************  
USGSImageLib::ImageOFile * ProjectedTIFFImageOFileFactory::
create(std::string & filename,
       const int & width, const int & height, const int & photo)
  throw (ImageOFactoryException)
{

  //check proj info
  if (!proj)
    throw ImageOFactoryException(TIFFPROJ_PROJNOTSET);
  
  if (!tiepoints || tiepointnum < 6)
    throw ImageOFactoryException(TIFFPROJ_BADTIE);

  //write the world file
  writeTFW(filename); 

  //create the tiff
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
    throw ImageOFactoryException(TIFFPROJ_NOMEM); //throw error
  }
  catch(...)
  {    
    throw ImageOFactoryException(TIFFPROJ_UNKOWN);//throw unkown
  }
}

//*************************************************************************
USGSImageLib::ImageOFile * ProjectedTIFFImageOFileFactory::
create(std::string & filename,
       const int & width, const int & height, const int & photo,
       ProjLib::Projection * inproj, const MathLib::Point & inscale,
       const double * intiepoints, const int & intiepointnum)
  throw(ImageOFactoryException)
{
  //set the projection
  set_projection_info(inproj, inscale, intiepoints, intiepointnum);

  //call other create function
  return create(filename, width, height, photo); 
}
  
//************************************************************************
void ProjectedTIFFImageOFileFactory::
writeTFW(const std::string & filename) throw (ImageOFactoryException)
{
  std::string tfwname;                            // name for the world file
 
  std::ofstream outfile;
  int dotpos;


  try
  {
    dotpos = filename.find_first_of(".");        //find the dot in the name
    
    if (dotpos == -1)
      throw ImageOFactoryException(TIFFPROJ_BADFILE);
                                                 //append the tfw to filename
    tfwname = filename.substr(0, dotpos) + std::string(".tfw"); 
                                                 //open the file
    outfile.open(tfwname.c_str(), std::ios::out | std::ios::trunc);
          
    if (outfile.fail())                          //check for error opening  
    {
      outfile.close();
      throw ImageOFactoryException(TIFFPROJ_NOTFW);
    }

    outfile.setf(std::ios::fixed);               //fixed point
    outfile.precision(5);                                        
    outfile << scale.x << std::endl;             //xscale
    outfile << "0.00000" << std::endl;           //no rotation
    outfile << "0.00000" << std::endl;           //(prewarped)
    outfile << ( -1.0 * scale.y) << std::endl;   //-yscale
    outfile << tiepoints[3] << std::endl;        //upper left x
    outfile << tiepoints[4] << std::endl;        //upper left y
    
    outfile.close();
  
  }
  catch (...)
  {
    outfile.close();
    throw ImageOFactoryException(TIFFPROJ_NOTFW);    
  }
}

#endif






