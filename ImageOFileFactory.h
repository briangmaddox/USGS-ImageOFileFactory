/***************************************************************************
                          imageofilefactory.h  -  description
                             -------------------
    begin                : Wed Oct 18 2000
    copyright            : (C) 2000 by Chris Bilderback
    email                : cbilderback@usgs.gov
 ***************************************************************************/



#ifndef IMAGEOFILEFACTORY_H
#define IMAGEOFILEFACTORY_H

#include <list>
#include <string>

#include "ImageOFile.h"
#include "ImageOFactoryException.h"


namespace ImageOFactory
{

/**Base Class that creates the image file
  *@author Chris Bilderback
  */
class ImageOFileFactory
{
public: 
  /** Main constructor for the class */
  ImageOFileFactory();
  
  /** Main Destructor for class */
  virtual ~ImageOFileFactory();

  /** Base function that creates the image files */
  virtual USGSImageLib::ImageOFile * create(std::string & filename,
    const int & width, const int & height, const int & photo)
    throw (ImageOFactoryException) = 0;

  /** flush function flushes all open files */
  void flush() throw();

  /** close function closes the specified image file and removes it from
      the list.  If the file is in the list and is closed the function
      returns true.  It returns false if the function is not in the list */
  bool close(USGSImageLib::ImageOFile * remove) throw();


protected: // Protected attributes and functions

  /** This is the list of all open files created by this object. */
  std::list<USGSImageLib::ImageOFile *> filelist;

};

} //namespace
#endif
