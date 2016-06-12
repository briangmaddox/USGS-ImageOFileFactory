/***************************************************************************
                          imageofilefactory.cpp  -  impelments factory base
                             -------------------
    begin                : Wed Oct 18 2000
    copyright            : (C) 2000 by Chris Bilderback
    email                : cbilderback@usgs.gov
 ***************************************************************************/


#ifndef IMAGEOFILEFACTORY
#define IMAGEOFILEFACTORY


#include "ImageOFileFactory.h"

using namespace ImageOFactory;

//***************************************************************************
ImageOFileFactory::ImageOFileFactory()
{}

//***************************************************************************
ImageOFileFactory::~ImageOFileFactory()
{
  flush();                    //delete all members in the list
}

//***************************************************************************
void ImageOFileFactory::flush() throw()
{
  unsigned int counter;                     //counter for loop
  USGSImageLib::ImageOFile * temp = NULL;   //temporary pointer

  for (counter = 0; counter < filelist.size(); counter++)
  {
    temp = filelist.front();                //get the front
    delete temp;                            //delete it
    filelist.pop_front();                   //remove it from the list
  }

}

//***************************************************************************
bool ImageOFileFactory::close(USGSImageLib::ImageOFile * remove) throw()
{
  std::list<USGSImageLib::ImageOFile *>::iterator it;  //iterator


  for (it = filelist.begin(); it != filelist.end(); it++)
  {
    if ((*it) == remove)                               //point to same location
    {
      delete remove;                                   //close the file
      remove = NULL;                                   //prevent further access
      filelist.erase(it);                              //remove the pointer 
      return true;                                     //found and deleted
    }
  }

  return false;                                        //was not in the list
}






#endif






