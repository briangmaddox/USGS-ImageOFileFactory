#ifndef GEOTIFFIMAGEOFILEFACTORY_H_
#define GEOTIFFIMAGEOFILEFACTORY_H_


#include "ProjectedImageOFileFactory.h"
#include "GeoTIFFImageOFile.h"
#include "ProjectionLib/UTMProjection.h"
#include "ProjectionLib/StatePlaneProjection.h"
#include "ProjectionLib/AlbersConicProjection.h"
#include "ProjectionLib/LambertConformalConicProjection.h"
#include "ProjectionLib/MercatorProjection.h"
#include "ProjectionLib/PolarStereographicProjection.h"
#include "ProjectionLib/PolyconicProjection.h"
#include "ProjectionLib/EquidistantConicProjection.h"
#include "ProjectionLib/TransverseMercatorProjection.h"
#include "ProjectionLib/StereographicProjection.h"
#include "ProjectionLib/LambertAzimuthalProjection.h"
#include "ProjectionLib/AzimuthalEquidistantProjection.h"
#include "ProjectionLib/GnomonicProjection.h"
#include "ProjectionLib/OrthographicProjection.h"
#include "ProjectionLib/SinusoidalProjection.h"
#include "ProjectionLib/EquirectangularProjection.h"
#include "ProjectionLib/MillerCylindricalProjection.h"
#include "ProjectionLib/VanDerGrintenProjection.h"
#include "ProjectionLib/RobinsonProjection.h"
#include "ProjectionLib/AlaskaConformalProjection.h"
#include "ProjectionLib/HotineObliqueMercatorProjection.h"
#include "ProjectionLib/GeographicProjection.h"
#include "ProjectionLib/GoodeHomolosineProjection.h"
#include <list>
#include <string>

/** Define some error messages for use exceptions thrown by
    this class */
#define GEOPROJ_PROJNOTSET  "GeoTIFFImageOFileFactory: Projection not set"
#define GEOPROJ_BADTIE      "GeoTIFFImageOFileFactory: Bad tiepoints"
#define GEOPROJ_UNKOWN      "GeoTIFFImageOFileFactory: Unkown Projection"
#define GEOPROJ_UNKOWNERR   "GeoTIFFImageOFileFactory: Unkown Error"
#define GEOPROJ_UNKOWNDATUM "GeoTIFFImageOFileFactory: Unkown Datum"
#define GEOPROJ_UNKOWNUNITS "GeoTIFFImageOFileFactory: Unkown Units"
#define GEOPROJ_GEOMAPFAIL  "GeoTIFFImageOFileFactory: GEO map failed"
#define GEOPROJ_UTMMAPFAIL  "GeoTIFFImageOFileFactory: UTM map failed"
#define GEOPROJ_SPCSMAPFAIL "GeoTIFFImageOFileFactory: SPCS map failed"
#define GEOPROJ_ALBERSMAPFAIL "GeoTIFFImageOFileFactory: ALBERS map failed"
#define GEOPROJ_MAPAZMFAIL  "GeoTIFFImageOFileFactory: AZM map failed"
#define GEOPROJ_PSEUDOCYMAPFAIL "GeoTIFFImageOFileFactory: PSUEDOCY map failed"
#define GEOPROJ_EQUIDFAIL   "GeoTIFFImageOFileFactory: EQUID map failed"
#define GEOPROJ_EQRECTMAPFAIL "GeoTIFFImageOFileFactory: EQRECT map failed"
#define GEOPROJ_HOMMAPFAIL  "GeoTIFFImageOFileFactory: HOM map failed"
#define GEOPROJ_LAMCCMAPFAIL "GeoTIFFImageOFileFactory: LAMCC map failed"
#define GEOPROJ_MERCATMAPFAIL "GeoTIFFImageOFileFactory: MERCAT map failed"
#define GEOPROJ_POLYCMAPFAIL "GeoTIFFImageOFileFactory: POLYC map failed"
#define GEOPROJ_PSMAPFAIL "GeoTIFFImageOFileFactory: PS map failed"
#define GEOPROJ_TMMAPFAIL "GeoTIFFImageOFileFactory: TM map failed"
#define GEOPROJ_VGRINTMAPFAIL "GEOTIFFImageOFileFactory: VGRINT map failed"




namespace ImageOFactory
{

/** geotiff image factory creates projected images. */
class GeoTIFFImageOFileFactory : public ProjectedImageOFileFactory
{
public:
  /** Main constructor for the class */
  GeoTIFFImageOFileFactory() throw (ImageOFactoryException);

  /** Class destructor */
  ~GeoTIFFImageOFileFactory();

  /** Overloaded create function creates a geo tiff based
      on which projection is set.  If no projection is 
      set then this will throw an error. */
  virtual USGSImageLib::ImageOFile * create(std::string  filename,
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
  
  /** Converts a projlib datum to a geotiff code */ 
  short unsigned int getGeoDatum(ProjLib::DATUM indatum) 
    throw(ImageOFactoryException);
  
  /** gets the geotiff unit code from the projlib unit type */
  short unsigned int getGeoUnit(ProjLib::UNIT inunit) 
    throw(ImageOFactoryException);

  /** gets the geotiff projection code from the projlib unit type */
  short unsigned int getGeoProj(ProjLib::PROJSYS insys) 
    throw(ImageOFactoryException);

  /** converts DMS */ 
  double ConvertFromDMS(double dms) throw();

  /** writeScale writes the scale to the file */
  void writeScale(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapGEO maps the geo proj to a file */
  void mapGEO(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapUTM maps the utm proj to a file */
  void mapUTM(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapSPCS maps the state plane proj to a file */
  void mapSPCS(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapALBERS maps the albers proj to a file */
  void mapALBERS(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapAzimuthal maps a Azimuthal projection */
  void mapAzimuthal(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapPseudocylinder maps a pseudocylinder to a file */
  void mapPseudocylinder(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapEQUIDC maps a EquidistantConicProjection to a file */
  void mapEQUIDC(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapEQRECT maps a EquirectangularProjection to a file */
  void mapEQRECT(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapHOM maps a HotineObliqueMercatorProjection to file */
  void mapHOM(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);

  /** mapLAMCC maps a lambert conformal conic projection to a file */
  void mapLAMCC(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapMERCAT maps a mercator projection to a file */
  void mapMERCAT(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);

  /** mapPOLYC maps a poly conic projection to a file */
  void mapPOLYC(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);

  /** mapPS maps a polar stereo graphic projection to a file */
  void mapPS(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);

  /** mapTM maps a transverse mercator projection to a file */
  void mapTM(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);
  
  /** mapVGRINT maps a vgrint projection to a file */
  void mapVGRINT(USGSImageLib::GeoTIFFImageOFile * outfile)
    throw (ImageOFactoryException);


};


//************************************************************
inline void GeoTIFFImageOFileFactory::
writeScale(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  //write the scale
  double OutScale[3] = {0, 0, 0};
  OutScale[0] = scale.x;
  OutScale[1] = scale.y;
  OutScale[2] = scale.z;
  
  outfile->setPixelScale(OutScale);
}



} //namespace

#endif



