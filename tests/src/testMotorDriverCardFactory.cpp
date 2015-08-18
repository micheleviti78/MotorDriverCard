#define BOOST_TEST_MODULE MotorDriverCardFactoryTest
#include <boost/test/included/unit_test.hpp>
using namespace boost::unit_test_framework;

#include "MotorDriverCardFactory.h"
#include <MtcaMappedDevice/devMap.h>
#include "DFMC_MD22Constants.h"
#include "MotorDriverCardDummy.h"


#include "testConfigConstants.h"

using namespace mtca4u;
using namespace mtca4u::dfmc_md22;

BOOST_AUTO_TEST_SUITE( MotorDriverCardFactoryTestSuite )

BOOST_AUTO_TEST_CASE( testInstance ){
  // check that the singleton pattern works
  MotorDriverCardFactory const & instance1 = MotorDriverCardFactory::instance();
  MotorDriverCardFactory const & instance2 = MotorDriverCardFactory::instance();

  BOOST_CHECK( &instance1 == &instance2);
}

BOOST_AUTO_TEST_CASE( testCreate ){
  // test the creation with devPcie, mapping file and device file not being the same

  // Prepare the dummy device. It has to have the correct firmware number in the
  // corresponding register.
  devMap<devPCIE> mappedMtcadummy;
  mappedMtcadummy.openDev(DUMMY_DEV_PATH, DUMMY_MOC_MAP);
  mappedMtcadummy.writeReg(PROJECT_VERSION_ADDRESS_STRING, MODULE_NAME_0,  &MINIMAL_FIRMWARE_VERSION);

  boost::shared_ptr<MotorDriverCard> motorDriverCard_PCIe1 = 
    MotorDriverCardFactory::instance().createMotorDriverCard(DUMMY_DEV_PATH, 
							     DUMMY_MOC_MAP,
                                                             MODULE_NAME_0,
							     CONFIG_FILE);

  boost::shared_ptr<MotorDriverCard> md22_dummy1 = 
    MotorDriverCardFactory::instance().createMotorDriverCard(MAP_FILE_NAME, 
							     MAP_FILE_NAME,
							     MODULE_NAME_0,
                                                             CONFIG_FILE);

  BOOST_CHECK( motorDriverCard_PCIe1.get() != md22_dummy1.get() );
  // there is one instance here and one in the factory
  BOOST_CHECK( md22_dummy1.use_count() == 2 );

  boost::shared_ptr<MotorDriverCard> md22_dummy2 = 
    MotorDriverCardFactory::instance().createMotorDriverCard(MAP_FILE_NAME, 
							     MAP_FILE_NAME,
                                                             MODULE_NAME_0,
							     CONFIG_FILE);
 
  BOOST_CHECK( md22_dummy1.get() == md22_dummy2.get() );
  // there are two instances here, and one in the factory
  BOOST_CHECK( md22_dummy1.use_count() == 3 );

  // change the firmware version to 0. Still 'creation' has to work because the
  // device must not be reopened but the same instance has to be used.
  mappedMtcadummy.writeReg(PROJECT_VERSION_ADDRESS_STRING, MODULE_NAME_0, &MINIMAL_FIRMWARE_VERSION);

  boost::shared_ptr<MotorDriverCard> motorDriverCard_PCIe2 = 
    MotorDriverCardFactory::instance().createMotorDriverCard(DUMMY_DEV_PATH, 
							     DUMMY_MOC_MAP,
                                                             MODULE_NAME_0,
							     CONFIG_FILE);
  
  BOOST_CHECK( motorDriverCard_PCIe1.get() == motorDriverCard_PCIe2.get() );
  // there are two instances here, and one in the factory
  BOOST_CHECK( motorDriverCard_PCIe1.use_count() == 3 );
}

BOOST_AUTO_TEST_CASE( testCreateDummy ){
  boost::shared_ptr<MotorDriverCardDummy> motorDriverCardDummy = 
    boost::dynamic_pointer_cast<MotorDriverCardDummy>(
      MotorDriverCardFactory::instance().createMotorDriverCard("/dummy/MotorDriverCard", 
							       "irrelevant",
                                                               MODULE_NAME_0,
							       "alsoIrrelevant"));
      
  BOOST_CHECK(motorDriverCardDummy);
}


BOOST_AUTO_TEST_SUITE_END()