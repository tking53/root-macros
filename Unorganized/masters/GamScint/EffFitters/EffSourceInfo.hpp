/** \file EffSourceInfo.hpp
 * \brief List of information about Eff sources
 *  @authors T.T. King
 */

#ifndef __EFF_SOURCE_INFO_HPP_
#define  __EFF_SOURCE_INFO_HPP_


namespace EffSources {

  //137Cs tapesource
  namespace CsTape {
    namespace Cs137 {

      static const double CalibratedActivity_ = 38.92 ; //nCi
      static const double DaysSinceCal_ = 1513;
      static const double RunTime_ = 16834*4 + 7479; //from 1808
      static const double HalfLifeDays_ = 30.08 * 365.25;
      static const double CurrentSourceActivity_ = CalibratedActivity_ * exp(-1*(log(2)/HalfLifeDays_)*DaysSinceCal_) ;

      namespace Line1 {
        static const double LineEnergy_ = 661.57;
        static const std::pair<double,double> HagFitRange_ = {600,750} ;
        static const std::pair<double,double> NaiFitRange_ = {0,0};
        static const double HagFHWMseed_ = 30.9;
        static const double NaiFHWMseed_ = 50.42;

        // .second is the area, CurrentSourceActivity(nCi) * 37 (bq/nCi) * absolute branching %
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CurrentSourceActivity_*37*0.851};
      } //end line 1
    }//end Cs137
  }//end CStape

  namespace CoTape{
    namespace Co60 {
      static const double CalibratedActivity_ = 35.69;
      static const double DaysSinceCal_ = 1513;
      static const double HalfLifeDays_ = 1925.28;
      static const double RunTime_ = 16384*5 + 7862; //from 1808  
      static const double CurrentSourceActivity_ = CalibratedActivity_ * exp(-1*(log(2)/HalfLifeDays_)*DaysSinceCal_) ;

      namespace Line1 { //1173
        static const double LineEnergy_ = 1173.28;
        static const std::pair<double,double> HagFitRange_ = {1100,1250};
        static const double HagFHWMseed_ = 46;
      
        static const std::pair<double,double> NaiFitRange_ = {0,0};
        static const double NaiFHWMseed_ = 60;

        // .second is the area, CurrentSourceActivity(nCi) * 37 (bq/nCi) * absolute branching %
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CurrentSourceActivity_*37*0.9985};
      } //end Line1

      namespace Line2 { //1332
        static const double LineEnergy_ = 1332.49 ;
        static const std::pair<double,double> HagFitRange_ = {1280,1380};
        static const double HagFHWMseed_ = 35;
      
        static const std::pair<double,double> NaiFitRange_ = {0,0};
        static const double NaiFHWMseed_ = 60;

        // .second is the area, CurrentSourceActivity(nCi) * 37 (bq/nCi) * absolute branching %
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CurrentSourceActivity_*37*0.9998};
      } //end line 2
    } //end 60Co
  } //end CoTape

  namespace BaEff{
    namespace Ba133 {
      static const double CalibratedActivity_ = 2760 ; //2.76MicroCurie
      static const double DaysSinceCal_ = 4692 ; //caled on 5/28/2003
      static const double HalfLifeDays_ = 10.551* 365.25 ;
      static const double RunTime_ = 1793; //from 1808  
      static const double CurrentSourceActivity_ = CalibratedActivity_ * exp(-1*(log(2)/HalfLifeDays_)*DaysSinceCal_) ;

      namespace Line1 { // 79.61 + 80.99 @ 2.65 and 32.9 respectivly 
        static const double LineEnergy_ = 75;
        static const std::pair<double,double> HagFitRange_ = {45,100};
        static const double HagFHWMseed_ = 21;
      
        static const std::pair<double,double> NaiFitRange_ = {0,0};
        static const double NaiFHWMseed_ = 50;

        // .second is the area, CurrentSourceActivity(nCi) * 37 (bq/nCi) * absolute branching %
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CurrentSourceActivity_*37*(0.329+0.0265)};
      } //end Line 1
    }//end Ba133
  }//end BaEff

  namespace MixedEu { //only listing ones relevant for HAGRiD and Nai. (see cal sheet in logbook for more info and for use with Clover)

    static const double DaysSinceCal_ = 1911 ;
    static const double RunTime_ = 6030 ;

    namespace Eu152 {
      static const double HalfLifeDays_ = 4964 ;

      namespace Line1 {
        static const double CalibratedLineRate_ = 2190;
        static const double LineEnergy_ = 344.3;
        static const std::pair<double,double> HagFitRange_ = {300,390};
        static const double HagFHWMseed_ = 24;
      
        // .second is the area, Eu is given in Photons/sec
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CalibratedLineRate_ * exp (-1*(log(2)/HalfLifeDays_)*DaysSinceCal_)};
      }  //end Line 1
    } //end Eu152
    namespace Eu154 {
      static const double HalfLifeDays_ = 3135.35;

      namespace Line1 {
        static const double CalibratedLineRate_ =  484 ;
        static const double LineEnergy_ = 591.7;
        static const std::pair<double,double> HagFitRange_ = {530,650};
        static const double HagFHWMseed_ = 34 ;
          // .second is the area, Eu is given in Photons/sec
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CalibratedLineRate_ * exp (-1*(log(2)/HalfLifeDays_)*DaysSinceCal_)};
      }  //end Line 1

      namespace Line2 {
        static const double CalibratedLineRate_ =  2070 ;
        static const double LineEnergy_ = 723.3;
        static const std::pair<double,double> HagFitRange_ = {680,760};
        static const double HagFHWMseed_ = 23 ;         
        // .second is the area, Eu is given in Photons/sec
        static const std::pair<double,double> PeakInfo_ = {LineEnergy_,CalibratedLineRate_ * exp (-1*(log(2)/HalfLifeDays_)*DaysSinceCal_)};
      } //end Line 2
    }  //end Eu154
  } //end MixedEu 
} //end EffSources
#endif
