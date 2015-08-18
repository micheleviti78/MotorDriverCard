#include "MotorControlerImpl.h"

#include "DFMC_MD22Constants.h"
using namespace mtca4u::dfmc_md22;
#include "TMC429Constants.h"
using namespace mtca4u::tmc429;

#include "MotorDriverCardImpl.h"


// just save some typing...
#define REG_OBJECT_FROM_SUFFIX( SUFFIX, moduleName )\
  mappedDevice->getRegisterAccessor( createMotorRegisterName( _id, SUFFIX ), moduleName )

// Macros which cover more than one line are not good for the code coverage test, as only the 
// macro call is checked. In this case it is probably ok because we gain a lot of code because
// we have to define many variables, AND there is only one line per function without braching,
// which is covered by the funcion coverage test (not the line coverage test).
// Otherwise we would need a complete test which checks the macro.
#define DEFINE_GET_SET_VALUE( NAME, IDX )\
  unsigned int MotorControlerImpl::get ## NAME (){\
    return _controlerSPI->read( _id, IDX ).getDATA();}\
  void MotorControlerImpl::set ## NAME (unsigned int value){\
    _controlerSPI->write( _id, IDX, value );}

#define DEFINE_SIGNED_GET_SET_VALUE( NAME, IDX , CONVERTER )\
  int MotorControlerImpl::get ## NAME (){\
    int readValue = static_cast<int>(_controlerSPI->read( _id, IDX ).getDATA());\
    return CONVERTER.customToThirtyTwo( readValue ); }\
  void MotorControlerImpl::set ## NAME (int value){\
  unsigned int writeValue = static_cast<unsigned int>(\
      CONVERTER.thirtyTwoToCustom( value ) ); \
      _controlerSPI->write( _id, IDX, writeValue );}

#define DEFINE_SET_GET_TYPED_CONTROLER_REGISTER( NAME )\
  NAME MotorControlerImpl::get ## NAME (){\
    return readTypedRegister< NAME >();}\
  void MotorControlerImpl::set ## NAME ( NAME const & inputWord ){\
    writeTypedControlerRegister( inputWord );}

#define DEFINE_SET_GET_TYPED_DRIVER_DATA( NAME, LOCAL_DATA_INSTANCE )\
  NAME const & MotorControlerImpl::get ## NAME () const{\
    return LOCAL_DATA_INSTANCE;}\
  void MotorControlerImpl::set ## NAME ( NAME const & driverData){\
    setTypedDriverData< NAME >( driverData, LOCAL_DATA_INSTANCE );}

namespace mtca4u
{

  MotorControlerImpl::MotorControlerImpl( unsigned int ID,
		  boost::shared_ptr< devMap<devBase> > const & mappedDevice,
                  std::string const & moduleName,
                  boost::shared_ptr< TMC429SPI > const & controlerSPI,
		  MotorControlerConfig const & motorControlerConfig ) 
    : _mappedDevice(mappedDevice), _id(ID),
      _controlerStatus(mappedDevice->getRegisterAccessor( CONTROLER_STATUS_BITS_ADDRESS_STRING, moduleName )),
      _actualPosition( REG_OBJECT_FROM_SUFFIX(  ACTUAL_POSITION_SUFFIX, moduleName ) ),
      _actualVelocity( REG_OBJECT_FROM_SUFFIX( ACTUAL_VELOCITY_SUFFIX, moduleName ) ),
      _actualAcceleration( REG_OBJECT_FROM_SUFFIX( ACTUAL_ACCELETATION_SUFFIX, moduleName ) ),
      _microStepCount( REG_OBJECT_FROM_SUFFIX( MICRO_STEP_COUNT_SUFFIX, moduleName ) ),
      _stallGuardValue( REG_OBJECT_FROM_SUFFIX( STALL_GUARD_VALUE_SUFFIX, moduleName ) ),
      _coolStepValue( REG_OBJECT_FROM_SUFFIX( COOL_STEP_VALUE_SUFFIX, moduleName ) ),
      _status( REG_OBJECT_FROM_SUFFIX( STATUS_SUFFIX, moduleName ) ),
      _enabled( REG_OBJECT_FROM_SUFFIX( ENABLE_SUFFIX, moduleName ) ),
      _decoderReadoutMode( REG_OBJECT_FROM_SUFFIX( DECODER_READOUT_MODE_SUFFIX, moduleName ) ),
      _decoderPosition( REG_OBJECT_FROM_SUFFIX( DECODER_POSITION_SUFFIX, moduleName ) ),
      _driverSPI( mappedDevice, moduleName, 
                  createMotorRegisterName(ID, SPI_WRITE_SUFFIX ),
		  createMotorRegisterName(ID, SPI_SYNC_SUFFIX ),
		  motorControlerConfig.driverSpiWaitingTime),
      _controlerSPI(controlerSPI),
      converter24bits(24), converter12bits(12)
  {
    setAccelerationThresholdData( motorControlerConfig.accelerationThresholdData );
    setActualPosition( motorControlerConfig.actualPosition );
    setChopperControlData( motorControlerConfig.chopperControlData );
    setCoolStepControlData( motorControlerConfig.coolStepControlData );
    setDecoderReadoutMode( motorControlerConfig.decoderReadoutMode );
    setDividersAndMicroStepResolutionData( motorControlerConfig.dividersAndMicroStepResolutionData );
    setDriverConfigData( motorControlerConfig.driverConfigData );
    setDriverControlData( motorControlerConfig.driverControlData );
    setInterruptData( motorControlerConfig.interruptData );
    setMaximumAcceleration( motorControlerConfig.maximumAcceleration );
    setMaximumVelocity( motorControlerConfig.maximumVelocity );
    setMicroStepCount( motorControlerConfig.microStepCount );
    setMinimumVelocity( motorControlerConfig.minimumVelocity );
    setPositionTolerance( motorControlerConfig.positionTolerance );
    setProportionalityFactorData( motorControlerConfig.proportionalityFactorData );
    setReferenceConfigAndRampModeData( motorControlerConfig.referenceConfigAndRampModeData );
    setStallGuardControlData( motorControlerConfig.stallGuardControlData );
    setTargetPosition( motorControlerConfig.targetPosition );
    setTargetVelocity( motorControlerConfig.targetVelocity );

    // enabling the motor is the last step after setting all registers
    setEnabled( motorControlerConfig.enabled );
  }

  unsigned int MotorControlerImpl::getID(){
    return _id;
  }
   
  int MotorControlerImpl::getActualPosition(){
    int readValue;
    _actualPosition->readReg( &readValue );
    return converter24bits.customToThirtyTwo( readValue );
  }

  unsigned int MotorControlerImpl::readRegObject( boost::shared_ptr< devMap<devBase>::RegisterAccessor > const & registerAccessor){
    int readValue;
    registerAccessor->readReg( &readValue );
    return static_cast<unsigned int>(readValue);
  }

  void MotorControlerImpl::setActualPosition(int position){
    _controlerSPI->write( _id, IDX_ACTUAL_POSITION,
			  static_cast<unsigned int>(converter24bits.thirtyTwoToCustom(position)) );
  }

  int MotorControlerImpl::getActualVelocity(){
    int readValue;
    _actualVelocity->readReg( &readValue );
    return converter12bits.customToThirtyTwo( readValue );
  }

  void MotorControlerImpl::setActualVelocity(int velocity){
    _controlerSPI->write( _id, IDX_ACTUAL_VELOCITY, 
			  static_cast<unsigned int>(converter12bits.thirtyTwoToCustom(velocity)) );
  }

  unsigned int MotorControlerImpl::getActualAcceleration(){
    return readRegObject( _actualAcceleration );
  }

  void MotorControlerImpl::setActualAcceleration(unsigned int acceleration){
    _controlerSPI->write( _id, IDX_ACTUAL_ACCELERATION, acceleration );
  }

  unsigned int MotorControlerImpl::getMicroStepCount(){
    return readRegObject( _microStepCount );
  }

  void MotorControlerImpl::setMicroStepCount(unsigned int microStepCount){
    _controlerSPI->write( _id, IDX_MICRO_STEP_COUNT, microStepCount );
  }

  unsigned int MotorControlerImpl::getCoolStepValue(){
    return readRegObject( _coolStepValue );
  }
 
  unsigned int MotorControlerImpl::getStallGuardValue(){
    return readRegObject( _stallGuardValue );
  }
 
  DriverStatusData MotorControlerImpl::getStatus(){
    return DriverStatusData( readRegObject( _status ) );
  }
 
  void MotorControlerImpl::setDecoderReadoutMode(unsigned int readoutMode){
    int32_t temporaryWriteWord = static_cast<int32_t>(readoutMode);
    _decoderReadoutMode->writeReg( &temporaryWriteWord );
  }

  unsigned int MotorControlerImpl::getDecoderReadoutMode(){
     return readRegObject( _decoderReadoutMode );
  }

  unsigned int MotorControlerImpl::getDecoderPosition(){
     return readRegObject( _decoderPosition );
  }
 
  void MotorControlerImpl::setEnabled(bool enable){
    int32_t enableWord = ( enable ? 1 : 0 );
    _enabled->writeReg( &enableWord );
  }

  bool MotorControlerImpl::isEnabled(){
     return readRegObject( _enabled );
  }

  DEFINE_SIGNED_GET_SET_VALUE( TargetPosition, IDX_TARGET_POSITION, converter24bits )
  DEFINE_GET_SET_VALUE( MinimumVelocity, IDX_MINIMUM_VELOCITY )
  DEFINE_GET_SET_VALUE( MaximumVelocity, IDX_MAXIMUM_VELOCITY )
  DEFINE_SIGNED_GET_SET_VALUE( TargetVelocity, IDX_TARGET_VELOCITY , converter12bits )
  DEFINE_GET_SET_VALUE( MaximumAcceleration, IDX_MAXIMUM_ACCELERATION )
  DEFINE_GET_SET_VALUE( PositionTolerance, IDX_DELTA_X_REFERENCE_TOLERANCE )
  DEFINE_GET_SET_VALUE( PositionLatched, IDX_POSITION_LATCHED )

  template<class T>
  T  MotorControlerImpl::readTypedRegister(){
    T typedWord;
    typedWord.setSMDA( _id );
    TMC429OutputWord readbackWord = _controlerSPI->read( _id, typedWord.getIDX_JDX());
    typedWord.setDATA( readbackWord.getDATA() );
    return typedWord;
  }

  void MotorControlerImpl::writeTypedControlerRegister(TMC429InputWord inputWord){
    // set/overwrite the id with this motors id
    inputWord.setSMDA( _id );
    _controlerSPI->write( inputWord );
  }

  DEFINE_SET_GET_TYPED_CONTROLER_REGISTER( AccelerationThresholdData )
  DEFINE_SET_GET_TYPED_CONTROLER_REGISTER( ProportionalityFactorData )
  DEFINE_SET_GET_TYPED_CONTROLER_REGISTER( ReferenceConfigAndRampModeData )
  DEFINE_SET_GET_TYPED_CONTROLER_REGISTER( InterruptData )
  DEFINE_SET_GET_TYPED_CONTROLER_REGISTER( DividersAndMicroStepResolutionData )

  DEFINE_SET_GET_TYPED_DRIVER_DATA( DriverControlData, _driverControlData)
  DEFINE_SET_GET_TYPED_DRIVER_DATA( ChopperControlData, _chopperControlData)
  DEFINE_SET_GET_TYPED_DRIVER_DATA( CoolStepControlData, _coolStepControlData)
  DEFINE_SET_GET_TYPED_DRIVER_DATA( StallGuardControlData, _stallGuardControlData)
  DEFINE_SET_GET_TYPED_DRIVER_DATA( DriverConfigData, _driverConfigData)

  template <class T>
  void MotorControlerImpl::setTypedDriverData(T const & driverData, T & localDataInstance){
    // FIXME: protect the following section by mutex
    _driverSPI.write( driverData.getDataWord() );
    // Remember the written word for readback.
    localDataInstance  = driverData;
        
    // FIXME: End of mutex protected section
  }

  MotorReferenceSwitchData MotorControlerImpl::getReferenceSwitchData(){
    // the bit pattern for the active flags
    unsigned int bitMask = 0x3 << 2*_id;
    
    unsigned int commonReferenceSwitchWord = _controlerSPI->read(SMDA_COMMON, JDX_REFERENCE_SWITCH).getDATA();
    unsigned int dataWord = (commonReferenceSwitchWord & bitMask) >> 2*_id;

    // the enabled flags
    MotorReferenceSwitchData motorReferenceSwitchData(dataWord);
    // note: the following code uses the implicit bool conversion to/from 0/1 to keep the code short.
    motorReferenceSwitchData.setNegativeSwitchEnabled( !getReferenceConfigAndRampModeData().getDISABLE_STOP_L() );
    motorReferenceSwitchData.setPositiveSwitchEnabled( !getReferenceConfigAndRampModeData().getDISABLE_STOP_R() );

    return motorReferenceSwitchData;
  }

  void MotorControlerImpl::setNegativeReferenceSwitchEnabled(bool enableStatus){
    // FIXME: protect the following section by mutex
    ReferenceConfigAndRampModeData referenceConfigAndRampModeData = getReferenceConfigAndRampModeData();

    referenceConfigAndRampModeData.setDISABLE_STOP_L(!enableStatus);

    setReferenceConfigAndRampModeData(referenceConfigAndRampModeData);

    // FIXME: End of mutex protected section
  }

  void MotorControlerImpl::setPositiveReferenceSwitchEnabled(bool enableStatus){
    // FIXME: protect the following section by mutex
    ReferenceConfigAndRampModeData referenceConfigAndRampModeData = getReferenceConfigAndRampModeData();

    referenceConfigAndRampModeData.setDISABLE_STOP_R(!enableStatus);

    setReferenceConfigAndRampModeData(referenceConfigAndRampModeData);

    // FIXME: End of mutex protected section
  }


  bool MotorControlerImpl::targetPositionReached(){
    int readValue;
    _controlerStatus->readReg( &readValue );
    TMC429StatusWord controlerStatusWord( readValue );

    return controlerStatusWord.getTargetPositionReached(_id);
  }

  unsigned int MotorControlerImpl::getReferenceSwitchBit(){
    int readValue;
    _controlerStatus->readReg( &readValue );
    TMC429StatusWord controlerStatusWord( readValue );

    return controlerStatusWord.getReferenceSwitchBit(_id);
  }

}// namespace mtca4u