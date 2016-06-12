#ifndef GEOTIFFIMAGEOFILEFACTORY_CPP_
#define GEOTIFFIMAGEOFILEFACTORY_CPP_



#include "GeoTIFFImageOFileFactory.h"

using namespace ImageOFactory;

//**********************************************************************
GeoTIFFImageOFileFactory::GeoTIFFImageOFileFactory() 
  throw (ImageOFactoryException)
  : ProjectedImageOFileFactory()
{}

//***********************************************************************
GeoTIFFImageOFileFactory::~GeoTIFFImageOFileFactory()
{}

//***********************************************************************
USGSImageLib::ImageOFile * GeoTIFFImageOFileFactory::
create(std::string filename,
       const int & width, const int & height, const int & photo)
  throw (ImageOFactoryException)
{
  USGSImageLib::ImageOFile * out;
  USGSImageLib::GeoTIFFImageOFile * outgeo;
  ProjLib::PROJSYS sys;
  //check proj info
  if (!proj)
    throw ImageOFactoryException(std::string(GEOPROJ_PROJNOTSET));
  
  if (!tiepoints || tiepointnum < 6)
    throw ImageOFactoryException(std::string(GEOPROJ_BADTIE));
  
  try
  {
    
    //first get the projsys and datum
    sys = proj->getProjectionSystem();   
    //go ahead and create the geotiff
    if (!(out = new (std::nothrow) USGSImageLib::GeoTIFFImageOFile
          (filename, width, height, photo)))
      throw std::bad_alloc();
    outgeo = dynamic_cast<USGSImageLib::GeoTIFFImageOFile*>(out);
    
    //set some geotiff defaults
    outgeo->setPlanarConfig(1);
    outgeo->setSubfileType(0);
    outgeo->setCompression(COMPRESSION_NONE);
    outgeo->setRowsPerStrip(1);

    //switch on the projection type
    switch(sys)
    {
    case ProjLib::GEO:                              //geographics
      mapGEO(outgeo); 
      break;
    case ProjLib::UTM:                               //UTM
      mapUTM(outgeo);
      break;
    case ProjLib::SPCS:                             //State plane
      mapSPCS(outgeo);
      break;
    case ProjLib::ALBERS:
      mapALBERS(outgeo);
      break;
    case ProjLib::AZMEQD:
    case ProjLib::GNOMON:
    case ProjLib::LAMAZ:
    case ProjLib::ORTHO:
    case ProjLib::STEREO:
      mapAzimuthal(outgeo);
      break;
    case ProjLib::MILLER:
    case ProjLib::ROBIN:
    case ProjLib::SNSOID:
      mapPseudocylinder(outgeo);
      break;
    case ProjLib::EQUIDC:
      mapEQUIDC(outgeo);
      break;
    case ProjLib::EQRECT:
      mapEQRECT(outgeo);
      break;
    case ProjLib::HOM:
      mapHOM(outgeo);
      break;
    case ProjLib::LAMCC:
      mapLAMCC(outgeo);
      break;
    case ProjLib::MERCAT:
      mapMERCAT(outgeo);
      break;
    case ProjLib::POLYC:
      mapPOLYC(outgeo);
      break;
    case ProjLib::PS:
      mapPS(outgeo);
      break;
    case ProjLib::ALASKA:
    case ProjLib::TM:
      mapTM(outgeo);
      break;
    case ProjLib::VGRINT:
      mapVGRINT(outgeo);
      break;
    default:
      throw ImageOFactoryException(GEOPROJ_UNKOWN);
      
    }
    filelist.push_back(out);                         //put it in the list
    return out;
  }
  catch(ImageOFactoryException & tempexception)
  {
    close(out);
    throw tempexception;
  }
  catch(...)
  {
    close(out);
    throw ImageOFactoryException(GEOPROJ_UNKOWNERR);
  }
}

//******************************************************************
USGSImageLib::ImageOFile * GeoTIFFImageOFileFactory::
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

//***************************************************************** 
short unsigned int GeoTIFFImageOFileFactory::
getGeoDatum(ProjLib::DATUM indatum) 
  throw(ImageOFactoryException)
{
  switch(indatum)
    {
    case ProjLib::ADINDAN: 
      return GCS_Adindan;
    case ProjLib::ARC1950:
      return GCS_Arc_1950;
    case ProjLib::ARC1960:
      return GCS_Arc_1960;
    case ProjLib::CAPE:
      return GCS_Cape;
    case ProjLib::EUROPEAN_DATUM_1950:
      return GCS_ED50;
    case ProjLib::GEODETIC_DATUM_1949:
      return GCS_GD49;
    case ProjLib::HU_TZU_SHAN:
      return GCS_Hu_Tzu_Shan;
    case ProjLib::INDIAN:
      return GCS_Indian_1975;
    case ProjLib::NAD27:
      return GCS_NAD27;
    case ProjLib::NAD83:
      return GCS_NAD83;
    case ProjLib::ORDNANCE_SURVEY_1936:
      return GCS_OSGB_1936;
    case ProjLib::PULKOVO_1942:
      return GCS_Pulkovo_1942;
    case ProjLib::PROVISIONAL_S_AMERICAN_1956:
      return GCS_PSAD56;
    case ProjLib::TOKYO:
      return GCS_Tokyo;
    case ProjLib::WGS_72:
      return GCS_WGS_72;
    case ProjLib::WGS_84:
      return GCS_WGS_84;
    default:
      throw ImageOFactoryException(GEOPROJ_UNKOWNDATUM);
    }
}
  
//*********************************************************
short unsigned int GeoTIFFImageOFileFactory::
getGeoUnit(ProjLib::UNIT inunit) 
  throw(ImageOFactoryException)
{
  switch ( inunit )
    {
    case ProjLib::METERS:
      return Linear_Meter;
     
    case ProjLib::US_FEET:
      return Linear_Foot;
    case ProjLib::RADIANS:
      return Angular_Radian;
    case ProjLib::ARC_DEGREES:
      return Angular_Degree;
    case ProjLib::ARC_SECONDS:
      return Angular_Arc_Second;
  
    default:
      throw ImageOFactoryException(GEOPROJ_UNKOWNUNITS);
    }
}

//****************************************************************
short unsigned int GeoTIFFImageOFileFactory::
getGeoProj(ProjLib::PROJSYS insys) 
  throw(ImageOFactoryException)
{
   switch(insys)
    {
    case ProjLib::GEO:
    case ProjLib::UTM://the first two are a special case
    case ProjLib::SPCS:
      return 0;
    case ProjLib::ALBERS:
      return CT_AlbersEqualArea;
    case ProjLib::AZMEQD:
      return CT_AzimuthalEquidistant;
    case ProjLib::GNOMON:
      return CT_Gnomonic;
    case ProjLib::LAMAZ:
      return CT_LambertAzimEqualArea;
    case ProjLib::ORTHO:
      return CT_Orthographic;
    case ProjLib::STEREO:
      return CT_Stereographic;
    case ProjLib::MILLER:
      return CT_MillerCylindrical;
    case ProjLib::ROBIN:
      return CT_Robinson;
    case ProjLib::SNSOID:
      return CT_Sinusoidal;
    case ProjLib::EQUIDC:
      return CT_EquidistantConic;
    case ProjLib::EQRECT:
      return CT_Equirectangular;
    case ProjLib::HOM:
      return CT_ObliqueMercator;
    case ProjLib::LAMCC:
      return CT_LambertConfConic_1SP;
    case ProjLib::MERCAT:
      return CT_Mercator;
    case ProjLib::POLYC:
      return CT_Polyconic;
    case ProjLib::PS:
      return CT_PolarStereographic;
    case ProjLib::ALASKA:
      return CT_AlaskaConformal;
    case ProjLib::TM:
      return CT_TransverseMercator;
    case ProjLib::VGRINT:
      return CT_VanDerGrinten;
    default:
      throw ImageOFactoryException(GEOPROJ_UNKOWN);
    }
}

//**************************************************************** 
double GeoTIFFImageOFileFactory::
ConvertFromDMS(double dms) throw()
{
  double sign = 1.0;
  double temp = dms;
  int deg;
  int min;
  if ( temp < 0.0 )
  {
    temp = -temp;
    sign = -1.0;
  }
  
  // Extract the degrees, minutes, and seconds
  deg = static_cast<int>(temp / 1000000);
  temp -= deg * 1000000;
  min = static_cast<int>(temp / 1000);
  temp -= min * 1000;
  temp /= 3600;
  temp += min / 60.0;
  temp += deg;
  return temp *sign;
}

//****************************************************************
void GeoTIFFImageOFileFactory::
mapGEO(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  try
  {
  //set keys for GEO
  outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short int>
                     (ModelTypeGeographic));
  outfile->setGeoKey(GTRasterTypeGeoKey, static_cast<unsigned short int>
                     (RasterPixelIsArea));
  outfile->setGeoKey(GeographicTypeGeoKey, getGeoDatum(proj->getDatum()));
  outfile->setGeoKey(GeogAngularUnitsGeoKey, getGeoUnit(proj->getUnit()));
  writeScale(outfile);
  outfile->setTiePoints(tiepoints, tiepointnum);
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_GEOMAPFAIL);
  }
}

//*****************************************************************
void GeoTIFFImageOFileFactory::
mapUTM(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  ProjLib::DATUM datum;            //temporary datum
  
  try
  {
    datum = proj->getDatum();        //get the datum
        
    if (datum == ProjLib::NAD83)     //check the datum
    {
      outfile->setUSGS_UTM(0, 0, 0, 0, 
                          dynamic_cast<ProjLib::UTMProjection *>
                           (proj)->getZone(),
                           1983);
    }        
    else
    {
      if (datum == ProjLib::NAD27)
        outfile->setUSGS_UTM(0, 0, 0, 0, 
                            dynamic_cast<ProjLib::UTMProjection *>
                             (proj)->getZone(),
                             1927);
      else
        throw ImageOFactoryException(GEOPROJ_UNKOWNDATUM);
    }
    outfile->setTiePoints(tiepoints, tiepointnum); 
    writeScale(outfile);             //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_UTMMAPFAIL);
  }
    
}
    
  
//****************************************************************
void GeoTIFFImageOFileFactory::
mapSPCS(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int unitcode;
  ProjLib::DATUM datum;
  
  try
  {
    unitcode = getGeoUnit(proj->getUnit()); //get a unit code
    datum = proj->getDatum();               //get the datum
    
    if (datum == ProjLib::NAD83)            //check for 1983 or 1927
      outfile->setUSGS_SPCS(0, 0, 0, 0, 
                 dynamic_cast<ProjLib::StatePlaneProjection *>
                            (proj)->getZone(),
                            1983);
    else
    {
      if (datum == ProjLib::NAD27)
      outfile->setUSGS_SPCS(0, 0, 0, 0, 
                   dynamic_cast<ProjLib::StatePlaneProjection *>
                            (proj)->getZone(),
                            1927);
      else
        throw ImageOFactoryException(GEOPROJ_UNKOWNDATUM);
    }
    //Write the unit code
    outfile->setGeoKey(GeogLinearUnitsGeoKey, unitcode);
    outfile->setTiePoints(tiepoints, tiepointnum);
    writeScale(outfile);             //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_SPCSMAPFAIL);
  }

}
 
//******************************************************
void GeoTIFFImageOFileFactory::
mapALBERS(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int projcode, datumcode, unitcode;      //geotiff codes
  double StdParallel1, StdParallel2, CenterLong,
    NatOriginLat, FalseEasting, FalseNorthing;
  ProjLib::AlbersConicProjection* albers =            //albers pointer
    dynamic_cast<ProjLib::AlbersConicProjection*>(proj);
  
  try
  {
    if (!albers)
      throw ImageOFactoryException();

    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    StdParallel1 = ConvertFromDMS(albers->getFirstStandardParallel());
    StdParallel2 = ConvertFromDMS(albers->getSecondStandardParallel());
    CenterLong = ConvertFromDMS(albers->getCentralMeridian());
    NatOriginLat = ConvertFromDMS(albers->getOriginLatitude());
    FalseEasting = albers->getFalseEasting();
    FalseNorthing = albers->getFalseNorthing();
    
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                       (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjStdParallel1GeoKey, StdParallel1);
    outfile->setGeoKey(ProjStdParallel2GeoKey, StdParallel2);
    outfile->setGeoKey(ProjNatOriginLatGeoKey,  NatOriginLat);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_ALBERSMAPFAIL);
  }
}

//***********************************************************
void GeoTIFFImageOFileFactory::
mapAzimuthal(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int projcode, datumcode, unitcode;  //geotiff codes
  double CenterLong, CenterLat, FalseEasting, FalseNorthing;
  ProjLib::AzimuthalProjection* azim =              //temp azimuthal proj
    dynamic_cast<ProjLib::AzimuthalProjection *>(proj);
  
  try
  {

    if (!azim)
      throw ImageOFactoryException();

    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    CenterLong = ConvertFromDMS(azim->getCentralLongitude());
    CenterLat = ConvertFromDMS(azim->getCentralLatitude());
    FalseEasting = azim->getFalseEasting();
    FalseNorthing = azim->getFalseNorthing();
    
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                       (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setGeoKey(ProjCenterLatGeoKey, CenterLat);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_MAPAZMFAIL);
  }
}

//*****************************************************************
void GeoTIFFImageOFileFactory::
mapPseudocylinder(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int projcode, datumcode, unitcode;//geotiff codes
  double CenterLong, FalseEasting, FalseNorthing;  //output paramerters
  ProjLib::PseudocylindricalProjection* pc = 
    dynamic_cast<ProjLib::PseudocylindricalProjection*>(proj);

  try
  {
    if (!pc)
      throw ImageOFactoryException();

    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    CenterLong = ConvertFromDMS(pc->getCentralMeridian());
    FalseEasting = pc->getFalseEasting();
    FalseNorthing = pc->getFalseNorthing();
    
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                       (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_PSEUDOCYMAPFAIL);
  }

}

//********************************************************
void GeoTIFFImageOFileFactory::
mapEQUIDC(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int projcode, datumcode, unitcode;
  double CenterLat, CenterLong, NatOriginLat,
    FalseEasting, FalseNorthing, StdParallel1,
    StdParallel2;
  ProjLib::EquidistantConicProjection* edc =
    dynamic_cast<ProjLib::EquidistantConicProjection*>(proj);
  
  try
  {

    if (!edc)
      throw ImageOFactoryException();

    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    CenterLat = ConvertFromDMS(edc->getStandardParallel());
    CenterLong = ConvertFromDMS(edc->getCentralMeridian());
    NatOriginLat = ConvertFromDMS(edc->getOriginLatitude());
    FalseEasting = edc->getFalseEasting();
    FalseNorthing = edc->getFalseNorthing();
    StdParallel1 = ConvertFromDMS(edc->getFirstStandardParallel());
    StdParallel2 = ConvertFromDMS(edc->getSecondStandardParallel());
    
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                       (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjStdParallel1GeoKey, StdParallel1);
    outfile->setGeoKey(ProjStdParallel2GeoKey, StdParallel2);
    outfile->setGeoKey(ProjNatOriginLatGeoKey,  NatOriginLat);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setGeoKey(ProjCenterLatGeoKey, CenterLat);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_EQUIDFAIL);
  }
}

//***************************************************************
void GeoTIFFImageOFileFactory::
mapEQRECT(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int projcode, datumcode, unitcode;
  double CenterLat, CenterLong, FalseEasting,
    FalseNorthing;
 
  ProjLib::EquirectangularProjection* eqrect =
    dynamic_cast<ProjLib::EquirectangularProjection*>(proj);

  try
  {
    if (!eqrect)
      throw ImageOFactoryException();

    CenterLat = ConvertFromDMS(eqrect->getTrueScaleLatitude());
    CenterLong = ConvertFromDMS(eqrect->getCentralMeridian());
    FalseEasting = eqrect->getFalseEasting();
    FalseNorthing = eqrect->getFalseNorthing();
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                       (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setGeoKey(ProjCenterLatGeoKey, CenterLat);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_EQRECTMAPFAIL);
  }
}


//***********************************************************************
void GeoTIFFImageOFileFactory::
mapHOM(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException) 
{
  double ScaleAtNatOrigin, AzimuthAngle,
    CenterLong, CenterLat, FalseEasting,
    FalseNorthing;
  short unsigned int datumcode, unitcode, projcode;

  try
  {

    ProjLib::HotineObliqueMercatorProjection* homerc =
      dynamic_cast<ProjLib::HotineObliqueMercatorProjection*>(proj);

    if (!homerc)
      throw ImageOFactoryException();

    // Make sure this is a type B HOM projection
    if ( 0.0 != homerc->getLatitude1()  
         || 0.0 != homerc->getLatitude2() 
         || 0.0 != homerc->getLongitude1() 
         || 0.0 != homerc->getLongitude2() 
         || 0.0 != homerc->getAzimuthPoint())
      throw ImageOFactoryException(GEOPROJ_UNKOWN);
    
    ScaleAtNatOrigin = homerc->getProjectionCenterScaleFactor();
    AzimuthAngle = homerc->getAzimuthAngle();
    CenterLong = ConvertFromDMS(homerc->getCentralMeridian());
    CenterLat = ConvertFromDMS(homerc->getOriginLatitude());
    FalseEasting = homerc->getFalseEasting();
    FalseNorthing = homerc->getFalseNorthing();
    
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                       (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setGeoKey(ProjCenterLatGeoKey, CenterLat);
    outfile->setGeoKey(ProjScaleAtNatOriginGeoKey, ScaleAtNatOrigin);
    outfile->setGeoKey(ProjAzimuthAngleGeoKey, AzimuthAngle);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_HOMMAPFAIL);
  }
}


//****************************************************************
void GeoTIFFImageOFileFactory::
mapLAMCC(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  short unsigned int projcode, datumcode,unitcode;
  double StdParallel1, StdParallel2,
    NatOriginLong, FalseOriginLat, FalseEasting,
    FalseNorthing;
  ProjLib::LambertConformalConicProjection* lamcc =
     dynamic_cast<ProjLib::LambertConformalConicProjection*>(proj);

  try
  {
    if (!lamcc)
      throw ImageOFactoryException();

    StdParallel1 = ConvertFromDMS(lamcc->getFirstStandardParallel());
    StdParallel2 = ConvertFromDMS(lamcc->getSecondStandardParallel());
    NatOriginLong = ConvertFromDMS(lamcc->getCentralMeridian());
    FalseOriginLat = ConvertFromDMS(lamcc->getOriginLatitude());
    FalseEasting = lamcc->getFalseEasting();
    FalseNorthing = lamcc->getFalseNorthing();
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());

    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                      (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                      (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjStdParallel1GeoKey, StdParallel1);
    outfile->setGeoKey(ProjStdParallel2GeoKey, StdParallel2);
    outfile->setGeoKey(ProjNatOriginLongGeoKey, NatOriginLong);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjFalseOriginLatGeoKey, FalseOriginLat);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_LAMCCMAPFAIL);
  }
}

/** mapMERCAT maps a mercator projection to a file */
void GeoTIFFImageOFileFactory::mapMERCAT
(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  double NatOriginLong, NatOriginLat, CenterEasting,
    CenterNorthing;
  unsigned short int projcode, datumcode, unitcode;
  ProjLib::MercatorProjection* merc =
    dynamic_cast<ProjLib::MercatorProjection*>(proj);
  
  try
  {
    if (!merc)
      throw ImageOFactoryException();
    
    NatOriginLong = ConvertFromDMS(merc->getCentralMeridian());
    NatOriginLat = ConvertFromDMS(merc->getOriginLatitude());
    CenterEasting = merc->getFalseEasting();
    CenterNorthing = merc->getFalseNorthing();
    
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                      (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                      (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjNatOriginLongGeoKey, NatOriginLong);
    outfile->setGeoKey(ProjNatOriginLatGeoKey,  NatOriginLat);
    outfile->setGeoKey(ProjCenterEastingGeoKey, CenterEasting);
    outfile->setGeoKey(ProjCenterNorthingGeoKey, CenterNorthing);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_MERCATMAPFAIL);
  }
}

//****************************************************************
void GeoTIFFImageOFileFactory::
mapPOLYC(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  unsigned short int projcode, datumcode, unitcode;
  double CenterLong, CenterLat, FalseEasting, FalseNorthing;
  ProjLib::PolyconicProjection* polycon =
    dynamic_cast<ProjLib::PolyconicProjection*>(proj);
  
  try
  {
    if (!polycon)
      throw ImageOFactoryException();

    CenterLong = ConvertFromDMS(polycon->getCentralMeridian());
    CenterLat = ConvertFromDMS(polycon->getOriginLatitude());
    FalseEasting = polycon->getFalseEasting();
    FalseNorthing = polycon->getFalseNorthing();
    
    
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    //now write out all parameters
    
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                      (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                      (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setGeoKey(ProjCenterLatGeoKey, CenterLat);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_POLYCMAPFAIL);
  }
}

//************************************************************
void GeoTIFFImageOFileFactory::
mapPS(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  double StraightVertPoleLong, NatOriginLat, FalseEasting,
    FalseNorthing;
  unsigned short int projcode, datumcode, unitcode;

  ProjLib::PolarStereographicProjection* polar = 
          dynamic_cast<ProjLib::PolarStereographicProjection*>(proj); 
       

  try
  {
    if (!polar)
      throw ImageOFactoryException();


    StraightVertPoleLong = ConvertFromDMS(polar->getLongitudePole());
    NatOriginLat = ConvertFromDMS(polar->getTrueScaleLatitude());
    FalseEasting = polar->getFalseEasting();
    FalseNorthing = polar->getFalseNorthing();
    
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                      (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                      (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjNatOriginLatGeoKey,  NatOriginLat);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjStraightVertPoleLongGeoKey, StraightVertPoleLong);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_PSMAPFAIL);
  }
}

//************************************************************
void GeoTIFFImageOFileFactory::
mapTM(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  double ScaleAtNatOrigin, CenterLong, NatOriginLat, FalseEasting, 
    FalseNorthing;
  unsigned short int projcode, datumcode, unitcode;
  ProjLib::TransverseMercatorProjection* trans = 
    dynamic_cast<ProjLib::TransverseMercatorProjection*>(proj);

  try
  {
    
    if (!trans)
      throw ImageOFactoryException();

    ScaleAtNatOrigin = trans->getCentralMeridianScaleFactor();
    CenterLong = ConvertFromDMS(trans->getCentralMeridian());
    NatOriginLat = ConvertFromDMS(trans->getOriginLatitude());
    FalseEasting = trans->getFalseEasting();
    FalseNorthing = trans->getFalseNorthing();
    
    projcode = getGeoProj(proj->getProjectionSystem());
    datumcode = getGeoDatum(proj->getDatum());
    unitcode =  getGeoUnit(proj->getUnit());
    //now write out all parameters
    outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                      (ModelProjected));
    outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                      (RasterPixelIsArea));
    outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
    outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                      (32767));
    outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
    outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
    outfile->setGeoKey(ProjNatOriginLatGeoKey,  NatOriginLat);
    outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
    outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
    outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
    outfile->setGeoKey(ProjScaleAtNatOriginGeoKey, ScaleAtNatOrigin);
    outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
    writeScale(outfile);                                //write the scale
  }
  catch(...)
  {
    throw ImageOFactoryException(GEOPROJ_TMMAPFAIL);
  }
}

//************************************************************ 
void GeoTIFFImageOFileFactory::
mapVGRINT(USGSImageLib::GeoTIFFImageOFile * outfile)
  throw (ImageOFactoryException)
{
  double CenterLat, CenterLong, FalseEasting, FalseNorthing;
  unsigned short int projcode, datumcode, unitcode;
  ProjLib::VanDerGrintenProjection* vandg =
    dynamic_cast<ProjLib::VanDerGrintenProjection*>(proj);

   try
   {
     if (!vandg)
       throw ImageOFactoryException();
     
     CenterLat = ConvertFromDMS(vandg->getOriginLatitude());
     CenterLong =ConvertFromDMS(vandg->getCentralMeridian());
     FalseEasting = vandg->getFalseEasting();
     FalseNorthing = vandg->getFalseNorthing();
     
     projcode = getGeoProj(proj->getProjectionSystem());
     datumcode = getGeoDatum(proj->getDatum());
     unitcode =  getGeoUnit(proj->getUnit());
     //now write out all parameters
     outfile->setGeoKey(GTModelTypeGeoKey, static_cast<unsigned short>
                      (ModelProjected));
     outfile->setGeoKey(GTRasterTypeGeoKey,static_cast<unsigned short>
                       (RasterPixelIsArea));
     outfile->setGeoKey(GeographicTypeGeoKey, datumcode);
     outfile->setGeoKey(ProjectedCSTypeGeoKey,static_cast<unsigned short>
                       (32767));
     outfile->setGeoKey(ProjectionGeoKey, static_cast<unsigned short>
                       (32767));
     outfile->setGeoKey(ProjCoordTransGeoKey, projcode);
     outfile->setGeoKey(ProjLinearUnitsGeoKey, unitcode);
     outfile->setGeoKey(ProjFalseEastingGeoKey, FalseEasting);
     outfile->setGeoKey(ProjFalseNorthingGeoKey, FalseNorthing);
     outfile->setGeoKey(ProjCenterLongGeoKey, CenterLong);
     outfile->setGeoKey(ProjCenterLatGeoKey, CenterLat);
     outfile->setTiePoints(tiepoints, tiepointnum);     //write the tiepoints
     writeScale(outfile);                                //write the scale
   }
   catch(...)
   {
     throw ImageOFactoryException(GEOPROJ_VGRINTMAPFAIL);
   }
}


#endif

