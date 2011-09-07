#ifndef IMarlinTrack_h
#define IMarlinTrack_h

#include <cfloat>

#include "lcio.h"

#include "EVENT/TrackerHit.h"
#include "IMPL/TrackStateImpl.h"

#include "gearimpl/Vector3D.h"


#include <exception>

namespace MarlinTrk{
  
  
  /** Interface for generic tracks in MarlinTrk. The interface should provide the functionality to
   *  perform track finding and fitting. It is asssumed that the underlying implemetation will by 
   *  a Kalman Filter or a similar algorithm.
   *
   * @version $Id$
   * @author S.Aplin, F. Gaede DESY
   */
  
  class IMarlinTrack {
    
  public:
    
    /** boolean constant for defining backward direction - to be used for intitialise */
    static const bool backward = false ;
    
    /** boolean constant for defining backward direction - to be used for intitialise */
    static const bool forward  = ! backward  ;
    

   
    static  const int modeBackward = - 1 ;
    static  const int modeClosest  =   0 ;
    static  const int modeForward  = + 1 ;


    static const int success  = 0 ;  // no error
    static const int error = 1 ;
    static const int bad_intputs = 3 ;
    static const int no_intersection = 4 ; // no intersection found




    /**default d'tor*/
    virtual ~IMarlinTrack() {};
    
    /** add hit to track - the hits have to be added ordered in time ( i.e. typically outgoing )
     *  this order will define the direction of the energy loss used in the fit
     */
    virtual int addHit(EVENT::TrackerHit* hit) = 0 ;

    /** initialise the fit using the hits added up to this point -
     *  the fit direction has to be specified using IMarlinTrack::backward or IMarlinTrack::forward. 
     *  this is the order  wrt the order used in addHit() that will be used in the fit() 
     */
    virtual int initialise( bool fitDirection ) = 0 ; 

    /** initialise the fit with a track state, and z component of the B field in Tesla.
     *  the fit direction has to be specified using IMarlinTrack::backward or IMarlinTrack::forward. 
     *  this is the order that will be used in the fit().
     *  it is the users responsibility that the track state is consistent with the order
     *  of the hits used in addHit() ( i.e. the direction of energy loss )
     */
    virtual int initialise( const IMPL::TrackStateImpl& ts, double bfield_z, bool fitDirection ) = 0 ;


    /** perform the fit of all current hits, returns error code ( IMarlinTrack::success if no error ) .
     *  the fit will be performed  in the order specified at initialise() wrt the order used in addFit(), i.e.
     *  IMarlinTrack::backward implies fitting from the outside to the inside for tracks comming from the IP.
     */
    virtual int fit() = 0 ;
    

    /** update the current fit using the supplied hit, return code via int. Provides the Chi2 increment to the fit from adding the hit via reference. 
     *  the given hit will not be added if chi2increment > maxChi2Increment. 
     */
    virtual int addAndFit( EVENT::TrackerHit* hit, double& chi2increment, double maxChi2Increment=DBL_MAX ) = 0 ;
    
    

    // Track State Accessesors
    
    /** get track state, returning TrackState, chi2 and ndf via reference 
     */
    virtual int getTrackState( IMPL::TrackStateImpl& ts, double& chi2, int& ndf ) = 0 ;


    /** get track state at measurement associated with the given hit, returning TrackState, chi2 and ndf via reference 
     */
    virtual int getTrackState( EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf ) = 0 ;


    // PROPAGATORS 
    
    /** propagate the fit to the point of closest approach to the given point, returning TrackState, chi2 and ndf via reference    
     */
    virtual int propagate( const gear::Vector3D& point, IMPL::TrackStateImpl& ts, double& chi2, int& ndf ) = 0 ;


    /** propagate track state at measurement associated with the given hit, the fit to the point of closest approach to the given point,
     *  returning TrackState, chi2 and ndf via reference   
     */
    virtual int propagate( const gear::Vector3D& point, EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf ) = 0 ;
    


    // /** propagate the fit to next sensitive layer, returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference 
    //  */
    // virtual int propagateToNextLayer( IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& layerID , mode = 0 ) = 0 ;


    // /** propagate track state at measurement associated with the given hit, to next sensitive layer, 
    //  *	returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference
    //  / */
    // virtual int propagateToNextLayer( EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& layerID, int mode=0 ) = 0  ;
    

    /** propagate fit to numbered sensitive layer, returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference 
     */
    virtual int propagateToLayer( int layerID, IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& detElementID, int mode=0 ) = 0  ;

    /** propagate track state at measurement associated with the given hit, to numbered sensitive layer, 
     *  returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference 
     */
    virtual int propagateToLayer( int layerID, EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& detElementID, int mode=0 ) = 0  ;


    // EXTRAPOLATORS

    /** extrapolate the fit to the point of closest approach to the given point, returning TrackState, chi2 and ndf via reference   
     */
    virtual int extrapolate( const gear::Vector3D& point, IMPL::TrackStateImpl& ts, double& chi2, int& ndf ) = 0 ;

    /** extrapolate track state at measurement associated with the given hit, to the point of closest approach to the given point, 
     *	returning TrackState, chi2 and ndf via reference   
     */
    virtual int extrapolate( const gear::Vector3D& point, EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf ) = 0 ;

    // /** extrapolate the fit to next sensitive layer, returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference 
    //  */
    // virtual int extrapolateToNextLayer( IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& detElementID, int mode=0 ) = 0  ;

    // /** extrapolate track state at measurement associated with the given hit, to next sensitive layer, 
    //  *	returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference 
    //  */
    // virtual int extrapolateToNextLayer( EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& detElementID, int mode=0 ) = 0  ;

    /** extrapolate the fit to numbered sensitive layer, returning TrackState via provided reference 
     */
    virtual int extrapolateToLayer( int layerID, IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& detElementID, int mode=0 ) = 0  ;

    /** extrapolate track state at measurement associated with the given hit, to numbered sensitive layer, 
     *  returning TrackState, chi2, ndf and integer ID of sensitive detector element via reference 
     */
    virtual int extrapolateToLayer( int layerID, EVENT::TrackerHit* hit, IMPL::TrackStateImpl& ts, double& chi2, int& ndf, int& detElementID, int mode=0 ) = 0  ;


    // INTERSECTORS

    // /** extrapolate the fit to next sensitive layer, returning intersection point in global 
    //  *  coordinates and integer ID of the intersected sensitive detector element via reference 
    //  */
    // virtual int intersectionWithNextLayer( gear::Vector3D& point, int& detElementID, int mode=0 ) = 0  ;

    // /** extrapolate track state at measurement associated with the given hit, to next sensitive layer, 
    //  *  returning intersection point in global coordinates and integer ID of the intersected sensitive detector element via reference 
    //  */
    // virtual int intersectionWithNextLayer( EVENT::TrackerHit* hit, gear::Vector3D& point, int& detElementID, int mode=0 ) = 0  ;
    

    /** extrapolate the fit to numbered sensitive layer, returning intersection point in global coordinates and integer ID of the 
     *  intersected sensitive detector element via reference 
     */
    virtual int intersectionWithLayer( int layerID, gear::Vector3D& point, int& detElementID, int mode=0 ) = 0  ;
    
    /** extrapolate track state at measurement associated with the given hit, to numbered sensitive layer,
     *  returning intersection point in global coordinates and integer ID of the intersected sensitive detector element via reference 
     */
    virtual int intersectionWithLayer( int layerID, EVENT::TrackerHit* hit, gear::Vector3D& point, int& detElementID, int mode=0 ) = 0  ;
    
    
  protected:
    
  private:
    
    IMarlinTrack& operator=( const IMarlinTrack&) ; // disallow assignment operator 
    
  } ;
  
} // end of MarlinTrk namespace 

#endif

